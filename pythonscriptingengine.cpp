#include "pythonscriptingengine.h"
#include "mainwindow.h"
#include "dbmanager.h"
#include "dbelementstage.h"
#include "dbelementsubstage.h"
#include "dbevent.h"
#include "dbdefense.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

#ifdef PYTHON_ENABLED

// Python module for redirecting output (tee a consola + buffer)
static const char* OUTPUT_REDIRECTOR = R"PYTHON(
import sys, threading, traceback

# Guarda referencias a los streams originales ANTES de redirigir
_ORIG_STDOUT = getattr(sys, "__stdout__", None) or sys.stdout
_ORIG_STDERR = getattr(sys, "__stderr__", None) or sys.stderr

class _BaseRedirector:
    encoding = 'utf-8'
    errors = 'replace'
    newlines = None

    def __init__(self, tee=True, is_err=False):
        self._buf = []
        self._lock = threading.Lock()
        self._tee = tee
        self._is_err = is_err

    def write(self, text):
        try:
            if not isinstance(text, str):
                text = str(text)
            with self._lock:
                self._buf.append(text)
            if self._tee:
                try:
                    # Escribe al stream original sin recursión
                    if self._is_err and _ORIG_STDERR:
                        _ORIG_STDERR.write(text)
                        _ORIG_STDERR.flush()
                    elif _ORIG_STDOUT:
                        _ORIG_STDOUT.write(text)
                        _ORIG_STDOUT.flush()
                except Exception:
                    # Nunca propagar
                    traceback.print_exc(file=_ORIG_STDERR if _ORIG_STDERR else sys.__stderr__)
            return len(text)
        except Exception:
            try:
                traceback.print_exc(file=_ORIG_STDERR if _ORIG_STDERR else sys.__stderr__)
            except Exception:
                pass
            return 0

    def flush(self):
        # nada que hacer, pero no debe fallar
        return None

    def get_output(self):
        try:
            with self._lock:
                s = ''.join(self._buf)
                self._buf.clear()
            return s
        except Exception:
            try:
                traceback.print_exc(file=_ORIG_STDERR if _ORIG_STDERR else sys.__stderr__)
            except Exception:
                pass
            return ''

    # API tipo archivo que consultan algunas libs
    def writable(self): return True
    def readable(self): return False
    def seekable(self): return False
    def isatty(self): return False
    def fileno(self):
        raise OSError("No fileno for redirector")

    # Permite activar/desactivar tee en caliente
    def set_tee(self, enabled: bool):
        self._tee = bool(enabled)

class StdoutRedirector(_BaseRedirector):
    pass

class StderrRedirector(_BaseRedirector):
    pass

stdout_redirector = StdoutRedirector(tee=True, is_err=False)
stderr_redirector = StderrRedirector(tee=True, is_err=True)

sys.stdout = stdout_redirector
sys.stderr = stderr_redirector

import types
def _get_output_both():
    # concatena stdout y stderr y limpia ambos
    return stdout_redirector.get_output() + stderr_redirector.get_output()

def _set_tee_both(enabled: bool):
    stdout_redirector.set_tee(enabled)
    stderr_redirector.set_tee(enabled)

redirector = types.SimpleNamespace(
    get_output=_get_output_both,
    set_tee=_set_tee_both
)
)PYTHON";


// Helper function to convert QString to Python string
static PyObject* qStringToPyString(const QString &str) {
    return PyUnicode_FromString(str.toUtf8().constData());
}

// Helper function to convert Python object to QString
static QString pyObjectToQString(PyObject *obj) {
    if (obj == nullptr || obj == Py_None) {
        return QString();
    }
    
    PyObject *str = PyObject_Str(obj);
    if (str == nullptr) {
        return QString();
    }
    
    const char *cstr = PyUnicode_AsUTF8(str);
    QString result = QString::fromUtf8(cstr);
    Py_DECREF(str);
    return result;
}

// Marrta.get_stages() implementation (includes all stages, even fictitious ones)
static PyObject* marrta_get_stages(PyObject *self, PyObject *args) {
    Q_UNUSED(self);
    Q_UNUSED(args);
    
    try {
        Database::DbElementStage dbStage;
        QList<EtapaDefinition> stages;
        dbStage.list(stages, true);  // true = include all stages (even fictitious with id <= 0)
        
        PyObject *stagesList = PyList_New(stages.size());
        for (int i = 0; i < stages.size(); i++) {
            PyObject *stageDict = PyDict_New();
            PyDict_SetItemString(stageDict, "id", PyLong_FromLong(stages[i].id));
            PyDict_SetItemString(stageDict, "codigo", qStringToPyString(stages[i].codigo));
            PyDict_SetItemString(stageDict, "nombre", qStringToPyString(stages[i].nombre));
            PyDict_SetItemString(stageDict, "descripcion", qStringToPyString(stages[i].descripcion));
            PyDict_SetItemString(stageDict, "orden", PyLong_FromLong(stages[i].orden));
            
            PyList_SetItem(stagesList, i, stageDict);
        }
        
        return stagesList;
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "Error retrieving stages");
        return nullptr;
    }
}

// ========================================================================
// SUCESOS (Events) API
// ========================================================================

// Sucesos.get_all() - Get all events
static PyObject* sucesos_get_all(PyObject *self, PyObject *args) {
    Q_UNUSED(self);
    Q_UNUSED(args);
    
    try {
        QSqlQuery query;
        QString sql = R"""(
            SELECT s.id, s.codigo, s.nombre, s.descripcion, 
                   s.frecuencia, s.consecuencia, s.error_humano, 
                   s.base, s.activo, s.completado, s.riesgo,
                   s.etapa_id, s.subetapa_id,
                   e.nombre as etapa_nombre, e.codigo as etapa_codigo,
                   sub.nombre as subetapa_nombre
            FROM suceso_iniciador s
            LEFT JOIN etapas e ON s.etapa_id = e.id
            LEFT JOIN subetapas sub ON s.subetapa_id = sub.id
            ORDER BY s.id
        )""";
        
        if (!query.exec(sql)) {
            PyErr_SetString(PyExc_RuntimeError, query.lastError().text().toUtf8().constData());
            return nullptr;
        }
        
        PyObject *eventsList = PyList_New(0);
        
        while (query.next()) {
            PyObject *eventDict = PyDict_New();
            
            PyDict_SetItemString(eventDict, "id", PyLong_FromLong(query.value("id").toInt()));
            PyDict_SetItemString(eventDict, "codigo", qStringToPyString(query.value("codigo").toString()));
            PyDict_SetItemString(eventDict, "nombre", qStringToPyString(query.value("nombre").toString()));
            PyDict_SetItemString(eventDict, "descripcion", qStringToPyString(query.value("descripcion").toString()));
            PyDict_SetItemString(eventDict, "frecuencia", PyLong_FromLong(query.value("frecuencia").toInt()));
            PyDict_SetItemString(eventDict, "consecuencia", PyLong_FromLong(query.value("consecuencia").toInt()));
            PyDict_SetItemString(eventDict, "riesgo", PyLong_FromLong(query.value("riesgo").toInt()));
            PyDict_SetItemString(eventDict, "error_humano", PyBool_FromLong(query.value("error_humano").toBool()));
            PyDict_SetItemString(eventDict, "base", PyBool_FromLong(query.value("base").toBool()));
            PyDict_SetItemString(eventDict, "activo", PyBool_FromLong(query.value("activo").toBool()));
            PyDict_SetItemString(eventDict, "completado", PyBool_FromLong(query.value("completado").toBool()));
            PyDict_SetItemString(eventDict, "etapa_id", PyLong_FromLong(query.value("etapa_id").toInt()));
            PyDict_SetItemString(eventDict, "subetapa_id", PyLong_FromLong(query.value("subetapa_id").toInt()));
            PyDict_SetItemString(eventDict, "etapa_nombre", qStringToPyString(query.value("etapa_nombre").toString()));
            PyDict_SetItemString(eventDict, "etapa_codigo", qStringToPyString(query.value("etapa_codigo").toString()));
            PyDict_SetItemString(eventDict, "subetapa_nombre", qStringToPyString(query.value("subetapa_nombre").toString()));
            
            PyList_Append(eventsList, eventDict);
            Py_DECREF(eventDict);
        }
        
        return eventsList;
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "Error retrieving events");
        return nullptr;
    }
}

// Sucesos.get_by_stage(stage_id, substage_id=0) - Get events by stage
static PyObject* sucesos_get_by_stage(PyObject *self, PyObject *args) {
    Q_UNUSED(self);
    
    long stageId;
    long substageId = 0;
    if (!PyArg_ParseTuple(args, "l|l", &stageId, &substageId)) {
        return nullptr;
    }
    
    try {
        QSqlQuery query;
        QString sql;
        
        if (substageId > 0) {
            sql = R"""(
                SELECT s.id, s.codigo, s.nombre, s.descripcion, 
                       s.frecuencia, s.consecuencia, s.error_humano, 
                       s.base, s.activo, s.completado, s.riesgo,
                       s.etapa_id, s.subetapa_id,
                       e.nombre as etapa_nombre, e.codigo as etapa_codigo,
                       sub.nombre as subetapa_nombre
                FROM suceso_iniciador s
                LEFT JOIN etapas e ON s.etapa_id = e.id
                LEFT JOIN subetapas sub ON s.subetapa_id = sub.id
                WHERE s.etapa_id = :stageId AND s.subetapa_id = :substageId
                ORDER BY s.id
            )""";
            query.prepare(sql);
            query.bindValue(":stageId", (int)stageId);
            query.bindValue(":substageId", (int)substageId);
        } else {
            sql = R"""(
                SELECT s.id, s.codigo, s.nombre, s.descripcion, 
                       s.frecuencia, s.consecuencia, s.error_humano, 
                       s.base, s.activo, s.completado, s.riesgo,
                       s.etapa_id, s.subetapa_id,
                       e.nombre as etapa_nombre, e.codigo as etapa_codigo,
                       sub.nombre as subetapa_nombre
                FROM suceso_iniciador s
                LEFT JOIN etapas e ON s.etapa_id = e.id
                LEFT JOIN subetapas sub ON s.subetapa_id = sub.id
                WHERE s.etapa_id = :stageId
                ORDER BY s.id
            )""";
            query.prepare(sql);
            query.bindValue(":stageId", (int)stageId);
        }
        
        if (!query.exec()) {
            PyErr_SetString(PyExc_RuntimeError, query.lastError().text().toUtf8().constData());
            return nullptr;
        }
        
        PyObject *eventsList = PyList_New(0);
        
        while (query.next()) {
            PyObject *eventDict = PyDict_New();
            
            PyDict_SetItemString(eventDict, "id", PyLong_FromLong(query.value("id").toInt()));
            PyDict_SetItemString(eventDict, "codigo", qStringToPyString(query.value("codigo").toString()));
            PyDict_SetItemString(eventDict, "nombre", qStringToPyString(query.value("nombre").toString()));
            PyDict_SetItemString(eventDict, "descripcion", qStringToPyString(query.value("descripcion").toString()));
            PyDict_SetItemString(eventDict, "frecuencia", PyLong_FromLong(query.value("frecuencia").toInt()));
            PyDict_SetItemString(eventDict, "consecuencia", PyLong_FromLong(query.value("consecuencia").toInt()));
            PyDict_SetItemString(eventDict, "riesgo", PyLong_FromLong(query.value("riesgo").toInt()));
            PyDict_SetItemString(eventDict, "error_humano", PyBool_FromLong(query.value("error_humano").toBool()));
            PyDict_SetItemString(eventDict, "base", PyBool_FromLong(query.value("base").toBool()));
            PyDict_SetItemString(eventDict, "activo", PyBool_FromLong(query.value("activo").toBool()));
            PyDict_SetItemString(eventDict, "completado", PyBool_FromLong(query.value("completado").toBool()));
            PyDict_SetItemString(eventDict, "etapa_id", PyLong_FromLong(query.value("etapa_id").toInt()));
            PyDict_SetItemString(eventDict, "subetapa_id", PyLong_FromLong(query.value("subetapa_id").toInt()));
            PyDict_SetItemString(eventDict, "etapa_nombre", qStringToPyString(query.value("etapa_nombre").toString()));
            PyDict_SetItemString(eventDict, "etapa_codigo", qStringToPyString(query.value("etapa_codigo").toString()));
            PyDict_SetItemString(eventDict, "subetapa_nombre", qStringToPyString(query.value("subetapa_nombre").toString()));
            
            PyList_Append(eventsList, eventDict);
            Py_DECREF(eventDict);
        }
        
        return eventsList;
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "Error retrieving events by stage");
        return nullptr;
    }
}

// Sucesos module methods
static PyMethodDef sucesos_methods[] = {
    {"get_all", sucesos_get_all, METH_NOARGS, "Get all events with their details"},
    {"get_by_stage", sucesos_get_by_stage, METH_VARARGS, "Get events by stage (stage_id, substage_id=0)"},
    {nullptr, nullptr, 0, nullptr}
};

// ========================================================================
// DEFENSAS (Defenses) API
// ========================================================================

// Defensas.get_all(event_id=0) - Get all defenses
static PyObject* defensas_get_all(PyObject *self, PyObject *args) {
    Q_UNUSED(self);
    
    long eventId = 0;
    if (!PyArg_ParseTuple(args, "|l", &eventId)) {
        return nullptr;
    }
    
    try {
        QSqlQuery query;
        QString sql;
        
        if (eventId > 0) {
            sql = R"""(
                SELECT d.id, d.codigo, d.nombre, d.descripcion, 
                       d.robustez, d.tipo, d.activo, d.suceso_iniciador_id,
                       d.etapa_orden_id,
                       e.nombre as etapa_nombre, e.codigo as etapa_codigo
                FROM defensa d
                LEFT JOIN etapas e ON d.etapa_orden_id = e.id
                WHERE d.suceso_iniciador_id = :eventId
                ORDER BY d.id
            )""";
            query.prepare(sql);
            query.bindValue(":eventId", (int)eventId);
        } else {
            sql = R"""(
                SELECT d.id, d.codigo, d.nombre, d.descripcion, 
                       d.robustez, d.tipo, d.activo, d.suceso_iniciador_id,
                       d.etapa_orden_id,
                       e.nombre as etapa_nombre, e.codigo as etapa_codigo
                FROM defensa d
                LEFT JOIN etapas e ON d.etapa_orden_id = e.id
                ORDER BY d.id
            )""";
            query.prepare(sql);
        }
        
        if (!query.exec()) {
            PyErr_SetString(PyExc_RuntimeError, query.lastError().text().toUtf8().constData());
            return nullptr;
        }
        
        PyObject *defensesList = PyList_New(0);
        
        while (query.next()) {
            PyObject *defenseDict = PyDict_New();
            
            int tipo = query.value("tipo").toInt();
            QString tipoStr;
            switch(tipo) {
                case 0: tipoStr = "Reductor Frecuencia"; break;
                case 1: tipoStr = "Barrera"; break;
                case 2: tipoStr = "Reductor Consecuencia"; break;
                default: tipoStr = "Desconocido"; break;
            }
            
            PyDict_SetItemString(defenseDict, "id", PyLong_FromLong(query.value("id").toInt()));
            PyDict_SetItemString(defenseDict, "codigo", qStringToPyString(query.value("codigo").toString()));
            PyDict_SetItemString(defenseDict, "nombre", qStringToPyString(query.value("nombre").toString()));
            PyDict_SetItemString(defenseDict, "descripcion", qStringToPyString(query.value("descripcion").toString()));
            PyDict_SetItemString(defenseDict, "robustez", PyLong_FromLong(query.value("robustez").toInt()));
            PyDict_SetItemString(defenseDict, "tipo", PyLong_FromLong(tipo));
            PyDict_SetItemString(defenseDict, "tipo_str", qStringToPyString(tipoStr));
            PyDict_SetItemString(defenseDict, "activo", PyBool_FromLong(query.value("activo").toBool()));
            PyDict_SetItemString(defenseDict, "suceso_id", PyLong_FromLong(query.value("suceso_iniciador_id").toInt()));
            PyDict_SetItemString(defenseDict, "etapa_orden_id", PyLong_FromLong(query.value("etapa_orden_id").toInt()));
            PyDict_SetItemString(defenseDict, "etapa_nombre", qStringToPyString(query.value("etapa_nombre").toString()));
            PyDict_SetItemString(defenseDict, "etapa_codigo", qStringToPyString(query.value("etapa_codigo").toString()));
            
            PyList_Append(defensesList, defenseDict);
            Py_DECREF(defenseDict);
        }
        
        return defensesList;
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "Error retrieving defenses");
        return nullptr;
    }
}

// Defensas.get_by_type(tipo, event_id=0) - Get defenses by type
static PyObject* defensas_get_by_type(PyObject *self, PyObject *args) {
    Q_UNUSED(self);
    
    long tipo;
    long eventId = 0;
    if (!PyArg_ParseTuple(args, "l|l", &tipo, &eventId)) {
        return nullptr;
    }
    
    try {
        QSqlQuery query;
        QString sql;
        
        if (eventId > 0) {
            sql = R"""(
                SELECT d.id, d.codigo, d.nombre, d.descripcion, 
                       d.robustez, d.tipo, d.activo, d.suceso_iniciador_id,
                       d.etapa_orden_id,
                       e.nombre as etapa_nombre, e.codigo as etapa_codigo
                FROM defensa d
                LEFT JOIN etapas e ON d.etapa_orden_id = e.id
                WHERE d.tipo = :tipo AND d.suceso_iniciador_id = :eventId
                ORDER BY d.id
            )""";
            query.prepare(sql);
            query.bindValue(":tipo", (int)tipo);
            query.bindValue(":eventId", (int)eventId);
        } else {
            sql = R"""(
                SELECT d.id, d.codigo, d.nombre, d.descripcion, 
                       d.robustez, d.tipo, d.activo, d.suceso_iniciador_id,
                       d.etapa_orden_id,
                       e.nombre as etapa_nombre, e.codigo as etapa_codigo
                FROM defensa d
                LEFT JOIN etapas e ON d.etapa_orden_id = e.id
                WHERE d.tipo = :tipo
                ORDER BY d.id
            )""";
            query.prepare(sql);
            query.bindValue(":tipo", (int)tipo);
        }
        
        if (!query.exec()) {
            PyErr_SetString(PyExc_RuntimeError, query.lastError().text().toUtf8().constData());
            return nullptr;
        }
        
        PyObject *defensesList = PyList_New(0);
        
        while (query.next()) {
            PyObject *defenseDict = PyDict_New();
            
            int tipo = query.value("tipo").toInt();
            QString tipoStr;
            switch(tipo) {
                case 0: tipoStr = "Reductor Frecuencia"; break;
                case 1: tipoStr = "Barrera"; break;
                case 2: tipoStr = "Reductor Consecuencia"; break;
                default: tipoStr = "Desconocido"; break;
            }
            
            PyDict_SetItemString(defenseDict, "id", PyLong_FromLong(query.value("id").toInt()));
            PyDict_SetItemString(defenseDict, "codigo", qStringToPyString(query.value("codigo").toString()));
            PyDict_SetItemString(defenseDict, "nombre", qStringToPyString(query.value("nombre").toString()));
            PyDict_SetItemString(defenseDict, "descripcion", qStringToPyString(query.value("descripcion").toString()));
            PyDict_SetItemString(defenseDict, "robustez", PyLong_FromLong(query.value("robustez").toInt()));
            PyDict_SetItemString(defenseDict, "tipo", PyLong_FromLong(tipo));
            PyDict_SetItemString(defenseDict, "tipo_str", qStringToPyString(tipoStr));
            PyDict_SetItemString(defenseDict, "activo", PyBool_FromLong(query.value("activo").toBool()));
            PyDict_SetItemString(defenseDict, "suceso_id", PyLong_FromLong(query.value("suceso_iniciador_id").toInt()));
            PyDict_SetItemString(defenseDict, "etapa_orden_id", PyLong_FromLong(query.value("etapa_orden_id").toInt()));
            PyDict_SetItemString(defenseDict, "etapa_nombre", qStringToPyString(query.value("etapa_nombre").toString()));
            PyDict_SetItemString(defenseDict, "etapa_codigo", qStringToPyString(query.value("etapa_codigo").toString()));
            
            PyList_Append(defensesList, defenseDict);
            Py_DECREF(defenseDict);
        }
        
        return defensesList;
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "Error retrieving defenses by type");
        return nullptr;
    }
}

// Defensas module methods
static PyMethodDef defensas_methods[] = {
    {"get_all", defensas_get_all, METH_VARARGS, "Get all defenses (optional event_id filter)"},
    {"get_by_type", defensas_get_by_type, METH_VARARGS, "Get defenses by type (tipo, event_id=0). Tipo: 0=FreqReducer, 1=Barrier, 2=ConsReducer"},
    {nullptr, nullptr, 0, nullptr}
};

// ========================================================================
// ETAPAS API (existing)
// ========================================================================

// Etapa class implementation
static PyObject* etapa_imprimir(PyObject *self, PyObject *args) {
    Q_UNUSED(args);
    
    // Get the Etapa object data
    PyObject *idObj = PyObject_GetAttrString(self, "id");
    PyObject *codigoObj = PyObject_GetAttrString(self, "codigo");
    PyObject *nombreObj = PyObject_GetAttrString(self, "nombre");
    PyObject *descripcionObj = PyObject_GetAttrString(self, "descripcion");
    
    if (!idObj || !codigoObj || !nombreObj || !descripcionObj) {
        PyErr_SetString(PyExc_AttributeError, "Etapa object missing required attributes");
        return nullptr;
    }
    
    long stageId = PyLong_AsLong(idObj);
    QString codigo = pyObjectToQString(codigoObj);
    QString nombre = pyObjectToQString(nombreObj);
    QString descripcion = pyObjectToQString(descripcionObj);
    
    Py_DECREF(idObj);
    Py_DECREF(codigoObj);
    Py_DECREF(nombreObj);
    Py_DECREF(descripcionObj);
    
    // Get substages for this stage
    QList<SubetapaDefinition> substages;
    try {
        Database::DbElementSubstage dbSubstage(stageId);
        dbSubstage.list(substages);
    } catch (...) {
        // No substages or error
    }
    
    // Build substage IDs tuple
    PyObject *substageIds = PyTuple_New(substages.size());
    for (int i = 0; i < substages.size(); i++) {
        PyTuple_SetItem(substageIds, i, PyLong_FromLong(substages[i].id));
    }
    
    // Return tuple: (ID, código, nombre, descripción, (substage_ids...))
    PyObject *result = PyTuple_New(5);
    PyTuple_SetItem(result, 0, PyLong_FromLong(stageId));
    PyTuple_SetItem(result, 1, qStringToPyString(codigo));
    PyTuple_SetItem(result, 2, qStringToPyString(nombre));
    PyTuple_SetItem(result, 3, qStringToPyString(descripcion));
    PyTuple_SetItem(result, 4, substageIds);
    
    return result;
}

// Etapa class methods
static PyMethodDef etapa_methods[] = {
    {"imprimir", etapa_imprimir, METH_NOARGS, "Return tuple: (ID, codigo, nombre, descripcion, (substage_ids))"},
    {nullptr, nullptr, 0, nullptr}
};

// Etapa type definition
static PyTypeObject EtapaType = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "Etapa",                          // tp_name
    sizeof(PyObject),                 // tp_basicsize
    0,                                // tp_itemsize
    nullptr,                          // tp_dealloc
    0,                                // tp_vectorcall_offset
    nullptr,                          // tp_getattr
    nullptr,                          // tp_setattr
    nullptr,                          // tp_as_async
    nullptr,                          // tp_repr
    nullptr,                          // tp_as_number
    nullptr,                          // tp_as_sequence
    nullptr,                          // tp_as_mapping
    nullptr,                          // tp_hash
    nullptr,                          // tp_call
    nullptr,                          // tp_str
    nullptr,                          // tp_getattro
    nullptr,                          // tp_setattro
    nullptr,                          // tp_as_buffer
    Py_TPFLAGS_DEFAULT,               // tp_flags
    "Etapa object",                   // tp_doc
    nullptr,                          // tp_traverse
    nullptr,                          // tp_clear
    nullptr,                          // tp_richcompare
    0,                                // tp_weaklistoffset
    nullptr,                          // tp_iter
    nullptr,                          // tp_iternext
    etapa_methods,                    // tp_methods
};

// Etapas module methods
static PyMethodDef etapas_methods[] = {
    {"get_all", marrta_get_stages, METH_NOARGS, "Get all stages as list of Etapa objects"},
    {nullptr, nullptr, 0, nullptr}
};

// Database.query() implementation
static PyObject* database_query(PyObject *self, PyObject *args) {
    Q_UNUSED(self);
    
    const char *sql;
    if (!PyArg_ParseTuple(args, "s", &sql)) {
        return nullptr;
    }
    
    QSqlQuery query;
    
    if (!query.exec(QString::fromUtf8(sql))) {
        QString error = query.lastError().text();
        PyErr_SetString(PyExc_RuntimeError, error.toUtf8().constData());
        return nullptr;
    }
    
    // Build result as list of dictionaries
    PyObject *results = PyList_New(0);
    
    while (query.next()) {
        QSqlRecord record = query.record();
        PyObject *row = PyDict_New();
        
        for (int i = 0; i < record.count(); i++) {
            QString fieldName = record.fieldName(i);
            QVariant value = record.value(i);
            
            PyObject *pyValue;
            if (value.isNull()) {
                pyValue = Py_None;
                Py_INCREF(Py_None);
            } else if (value.typeId() == QMetaType::Int || value.typeId() == QMetaType::LongLong) {
                pyValue = PyLong_FromLongLong(value.toLongLong());
            } else if (value.typeId() == QMetaType::Double) {
                pyValue = PyFloat_FromDouble(value.toDouble());
            } else {
                pyValue = qStringToPyString(value.toString());
            }
            
            PyDict_SetItemString(row, fieldName.toUtf8().constData(), pyValue);
            Py_DECREF(pyValue);
        }
        
        PyList_Append(results, row);
        Py_DECREF(row);
    }
    
    return results;
}

// Database module methods
static PyMethodDef database_methods[] = {
    {"query", database_query, METH_VARARGS, "Execute SQL query and return results as list of dictionaries"},
    {nullptr, nullptr, 0, nullptr}
};

// Marrta module methods
static PyMethodDef marrta_module_methods[] = {
    {"get_version", [](PyObject*, PyObject*) -> PyObject* {
        return qStringToPyString("MARRTA 1.5");
    }, METH_NOARGS, "Get MARRTA version"},
    {nullptr, nullptr, 0, nullptr}
};

PythonScriptingEngine::PythonScriptingEngine(MainWindow *mainWindow)
    : mainWindow(mainWindow), consoleOutput(nullptr), mainModule(nullptr), 
      mainDict(nullptr), initialized(false)
{
    initialized = initializePython();
    if (initialized) {
        registerMarrtaObject();
        registerDatabaseObject();
        registerEtapasObject();
        registerSucesosObject();
        registerDefensasObject();
        redirectPythonOutput();
    }
}

PythonScriptingEngine::~PythonScriptingEngine()
{
    if (Py_IsInitialized()) {
        Py_Finalize();
    }
}

bool PythonScriptingEngine::initializePython()
{
    if (Py_IsInitialized()) {
        pythonInitError_ = QObject::tr("Python ya está inicializado");
        qWarning() << pythonInitError_;
        return false;
    }
    
    // Use PyConfig for better control and error handling (Python 3.8+)
    PyStatus status;
    PyConfig config;
    
    PyConfig_InitPythonConfig(&config);
    
    // Don't write bytecode files (.pyc)
    config.write_bytecode = 0;
    
    // Parse minimal config to avoid fatal errors
    config.parse_argv = 0;
    
    // Set site import to 0 to avoid importing site module which requires full stdlib
    config.site_import = 0;
    
    // Initialize with config
    status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    
    // Check if initialization succeeded
    if (PyStatus_Exception(status)) {
        pythonInitError_ = QObject::tr("Error al inicializar el intérprete de Python: %1").arg(QString::fromUtf8(status.err_msg ? status.err_msg : "desconocido"));
        qCritical() << pythonInitError_;
        // Don't show dialog here - will be shown when user opens Python editor
        return false;
    }
    
    if (!Py_IsInitialized()) {
        pythonInitError_ = QObject::tr("Error al inicializar el intérprete de Python");
        qCritical() << pythonInitError_;
        // Don't show dialog here - will be shown when user opens Python editor
        return false;
    }
    
    // Check for Python errors after initialization
    if (PyErr_Occurred()) {
        PyObject *exc_type, *exc_value, *exc_tb;
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        
        QString errorMsg;
        if (exc_value) {
            PyObject *str = PyObject_Str(exc_value);
            if (str) {
                const char *cstr = PyUnicode_AsUTF8(str);
                if (cstr) {
                    errorMsg = QString::fromUtf8(cstr);
                }
                Py_DECREF(str);
            }
            Py_XDECREF(exc_type);
            Py_XDECREF(exc_value);
            Py_XDECREF(exc_tb);
        }
        
        pythonInitError_ = QObject::tr("Error durante la inicialización de Python: %1").arg(errorMsg.isEmpty() ? "desconocido" : errorMsg);
        qCritical() << pythonInitError_;
        
        Py_Finalize();
        // Don't show dialog here - will be shown when user opens Python editor
        return false;
    }
    
    // Get main module
    mainModule = PyImport_AddModule("__main__");
    if (!mainModule) {
        pythonInitError_ = QObject::tr("Error al obtener el módulo __main__ de Python");
        qCritical() << pythonInitError_;
        
        // Check for Python error details
        if (PyErr_Occurred()) {
            PyObject *exc_type, *exc_value, *exc_tb;
            PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
            
            if (exc_value) {
                PyObject *str = PyObject_Str(exc_value);
                if (str) {
                    const char *cstr = PyUnicode_AsUTF8(str);
                    if (cstr) {
                        pythonInitError_ += QString(": %1").arg(QString::fromUtf8(cstr));
                    }
                    Py_DECREF(str);
                }
                Py_XDECREF(exc_type);
                Py_XDECREF(exc_value);
                Py_XDECREF(exc_tb);
            }
        }
        
        // Finalize Python before returning
        Py_Finalize();
        // Don't show dialog here - will be shown when user opens Python editor
        return false;
    }
    
    // Get main dictionary
    mainDict = PyModule_GetDict(mainModule);
    if (!mainDict) {
        pythonInitError_ = QObject::tr("Error al obtener el diccionario del módulo __main__");
        qCritical() << pythonInitError_;
        
        // Finalize Python before returning
        Py_Finalize();
        // Don't show dialog here - will be shown when user opens Python editor
        return false;
    }
    
    // Clear any previous error
    pythonInitError_.clear();
    
    return true;
}

void PythonScriptingEngine::registerMarrtaObject()
{
    // Create Marrta module
    static PyModuleDef marrtaModuleDef = {
        PyModuleDef_HEAD_INIT,
        "Marrta",
        "MARRTA base object for accessing application features",
        -1,
        marrta_module_methods
    };
    
    PyObject *marrtaModule = PyModule_Create(&marrtaModuleDef);
    
    if (marrtaModule) {
        PyDict_SetItemString(mainDict, "Marrta", marrtaModule);
        Py_DECREF(marrtaModule);
    }
}

void PythonScriptingEngine::registerDatabaseObject()
{
    // Create Database module
    static PyModuleDef databaseModuleDef = {
        PyModuleDef_HEAD_INIT,
        "Database",
        "Database access for SQL queries",
        -1,
        database_methods
    };
    
    PyObject *databaseModule = PyModule_Create(&databaseModuleDef);
    
    if (databaseModule) {
        PyDict_SetItemString(mainDict, "Database", databaseModule);
        Py_DECREF(databaseModule);
    }
}

void PythonScriptingEngine::registerEtapasObject()
{
    // Register Etapa type
    if (PyType_Ready(&EtapaType) < 0) {
        qWarning() << "Failed to initialize Etapa type";
        return;
    }
    
    // Create Etapas module with get_all method
    static PyModuleDef etapasModuleDef = {
        PyModuleDef_HEAD_INIT,
        "Etapas",
        "Access to stages (etapas)",
        -1,
        etapas_methods
    };
    
    PyObject *etapasModule = PyModule_Create(&etapasModuleDef);
    
    if (etapasModule) {
        // Add Etapa class to Etapas module
        Py_INCREF(&EtapaType);
        PyModule_AddObject(etapasModule, "Etapa", (PyObject*)&EtapaType);
        
        // Register module
        PyDict_SetItemString(mainDict, "Etapas", etapasModule);
        Py_DECREF(etapasModule);
        
        // Add helper to create Etapa objects from dictionaries
        const char* etapa_helper = R"PYTHON(
class Etapa:
    def __init__(self, data):
        self.id = data['id']
        self.codigo = data['codigo']
        self.nombre = data['nombre']
        self.descripcion = data['descripcion']
        self.orden = data['orden']
    
    def imprimir(self):
        """Return tuple: (ID, código, nombre, descripción, (substage_ids...))"""
        # Import here to avoid circular dependency
        substages = []
        try:
            results = Database.query(f"SELECT id FROM subetapas WHERE etapa_id = {self.id} ORDER BY orden")
            substages = [row['id'] for row in results]
        except:
            pass
        return (self.id, self.codigo, self.nombre, self.descripcion, tuple(substages))

# Override get_all to return Etapa objects
_original_get_all = Etapas.get_all
def get_all_etapas():
    stages_data = _original_get_all()
    return [Etapa(stage) for stage in stages_data]

Etapas.get_all = get_all_etapas
)PYTHON";
        PyRun_SimpleString(etapa_helper);
    }
}

void PythonScriptingEngine::redirectPythonOutput()
{
    PyRun_SimpleString(OUTPUT_REDIRECTOR);
}

QString PythonScriptingEngine::execute(const QString &code)
{
    if (!initialized) {
        return QString("Error: Motor de Python no inicializado");
    }
    
    // Execute the code
    PyObject *result = PyRun_String(
        code.toUtf8().constData(),
        Py_file_input,
        mainDict,
        mainDict
    );
    
    QString output;
    
    // Get redirected output
    PyObject *redirectorModule = PyDict_GetItemString(mainDict, "redirector");
    if (redirectorModule) {
        PyObject *getOutputMethod = PyObject_GetAttrString(redirectorModule, "get_output");
        if (getOutputMethod && PyCallable_Check(getOutputMethod)) {
            PyObject *outputObj = PyObject_CallObject(getOutputMethod, nullptr);
            if (outputObj) {
                output = pyObjectToQString(outputObj);
                Py_DECREF(outputObj);
            }
            Py_DECREF(getOutputMethod);
        }
    }
    
    // Check for errors
    if (result == nullptr) {
        PyObject *exc_type, *exc_value, *exc_tb;
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        
        if (exc_value) {
            QString error = pyObjectToQString(exc_value);
            output += "\n" + QString("Error: ") + error;
            Py_XDECREF(exc_type);
            Py_XDECREF(exc_value);
            Py_XDECREF(exc_tb);
        } else {
            output += "\n" + QString("Error desconocido al ejecutar el código");
        }
    } else {
        Py_DECREF(result);
    }
    
    return output;
}

void PythonScriptingEngine::setConsoleDestiny(QPlainTextEdit *destiny)
{
    consoleOutput = destiny;
}

QString PythonScriptingEngine::pythonInitializationError() const
{
    return pythonInitError_;
}

void PythonScriptingEngine::registerSucesosObject()
{
    // Create Sucesos module
    static PyModuleDef sucesosModuleDef = {
        PyModuleDef_HEAD_INIT,
        "Sucesos",
        "Access to events (sucesos iniciadores)",
        -1,
        sucesos_methods
    };
    
    PyObject *sucesosModule = PyModule_Create(&sucesosModuleDef);
    
    if (sucesosModule) {
        PyDict_SetItemString(mainDict, "Sucesos", sucesosModule);
        Py_DECREF(sucesosModule);
    }
}

void PythonScriptingEngine::registerDefensasObject()
{
    // Create Defensas module
    static PyModuleDef defensasModuleDef = {
        PyModuleDef_HEAD_INIT,
        "Defensas",
        "Access to defenses (barriers, frequency and consequence reducers)",
        -1,
        defensas_methods
    };
    
    PyObject *defensasModule = PyModule_Create(&defensasModuleDef);
    
    if (defensasModule) {
        PyDict_SetItemString(mainDict, "Defensas", defensasModule);
        Py_DECREF(defensasModule);
    }
}

#else // PYTHON_ENABLED not defined

// Stub implementations when Python is not available
PythonScriptingEngine::PythonScriptingEngine(MainWindow *mainWindow)
    : mainWindow(mainWindow), consoleOutput(nullptr), initialized(false)
{
    pythonInitError_ = QObject::tr("Soporte de scripting Python no compilado. Reconstruir con PYTHON_ENABLED=1");
    qWarning() << pythonInitError_;
}

PythonScriptingEngine::~PythonScriptingEngine()
{
}

bool PythonScriptingEngine::initializePython()
{
    return false;
}

void PythonScriptingEngine::registerMarrtaObject()
{
}

void PythonScriptingEngine::registerDatabaseObject()
{
}

void PythonScriptingEngine::registerEtapasObject()
{
}

void PythonScriptingEngine::registerSucesosObject()
{
}

void PythonScriptingEngine::registerDefensasObject()
{
}

void PythonScriptingEngine::redirectPythonOutput()
{
}

QString PythonScriptingEngine::execute(const QString &code)
{
    Q_UNUSED(code);
    return QString("Error: Python scripting support not compiled in.\n"
                   "To enable Python support, rebuild with:\n"
                   "qmake \"PYTHON_ENABLED=1\" \"PYTHON_INCLUDE=<path>\" \"PYTHON_LIB=<path>\"\n"
                   "Example (Windows): qmake \"PYTHON_ENABLED=1\" \"PYTHON_INCLUDE=C:/Python312/include\" \"PYTHON_LIB=C:/Python312/libs\"");
}

void PythonScriptingEngine::setConsoleDestiny(QPlainTextEdit *destiny)
{
    consoleOutput = destiny;
}

QString PythonScriptingEngine::pythonInitializationError() const
{
    return pythonInitError_;
}

#endif // PYTHON_ENABLED
