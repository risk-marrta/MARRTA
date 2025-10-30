#include "etapas.h"
#include "ui_etapas.h"

#include "subetapas.h"

#include <QListWidget>
#include <QListView>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <algorithm>

#include "dbmanager.h"

#include "dbelementstage.h"

Etapas::Etapas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Etapas)
{
    etapasModel = nullptr;
    ui->setupUi(this);

    try{
        auto stages = Database::DbElementStage();
        stages.list(etapasDb);

        //if (etapasDb.count() > 0) {
            fillModel();
            showSelectedItem();
            // Mensaje duplicado en caso de error
        /*} else {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Icon::Critical);
            msgBox.setText("No se han podido rescatar las etapas de la base de datos");
            msgBox.exec();
        }*/
    } catch (Database::DbException exception) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.setText(exception.message());
        msgBox.exec();
    }
}

Etapas::~Etapas()
{
    delete etapasModel;
    delete ui;
}

void Etapas::fillModel()
{
    if (etapasModel != nullptr) {
        delete etapasModel;
    }
    etapasModel = new QStandardItemModel(this);
    for(auto &model : etapasDb) {
        auto item = new QStandardItem(model.nombre);
        item->setData(QVariant::fromValue(model.orden));
        etapasModel->appendRow(item);
    }
    if (etapasDb.count() > 0) {
        selectedId = etapasDb.first().orden;
    } else {
        selectedId = -1;
    }
    ui->listaEtapas->setModel(etapasModel);

    connect(ui->listaEtapas->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        [=] (const QItemSelection &selected) {
            auto selectedIndexes = selected.indexes();
            // @moises: ahora cuando se deselecciona un item también llega aquí
            if (selectedIndexes.size() > 0 ){
                actualIndex = selectedIndexes.first();
                selectedId = etapasModel->itemData(actualIndex).last().toInt();
                showSelectedItem();
            }
        }
    );
}

void Etapas::showSelectedItem()
{
    ui->etSubetapas->setText("");

    if (selectedId >= 0) {
        ui->edNombre->setText(etapasDb[selectedId].nombre);
        ui->edDescripcion->setText(etapasDb[selectedId].descripcion);
        ui->edCodigo->setText(etapasDb[selectedId].codigo);

        ui->btnSubirOrden->setEnabled(etapasDb[selectedId].orden != etapasDb.begin()->orden);
        ui->btnBajarOrden->setEnabled(etapasDb[selectedId].orden != etapasDb.back().orden);
        QString subetapas;
        if (etapasDb[selectedId].subetapas.size() > 0) {
            for (auto subetapa : etapasDb[selectedId].subetapas)    {
                subetapas += "<li>" + subetapa + "</li>";
            }
            ui->etSubetapas->setText("<h3>Subetapas</h3><ul>" + subetapas + "</ul>");
        }

        if (etapasDb[selectedId].id < 0) {
            ui->edNombre->setReadOnly(true);
            ui->edDescripcion->setReadOnly(true);
            ui->edCodigo->setReadOnly(true);
        } else {
            ui->edNombre->setReadOnly(false);
            ui->edDescripcion->setReadOnly(false);
            ui->edCodigo->setReadOnly(false);
        }
    } else {
        ui->edNombre->setText("");
        ui->edDescripcion->setText("");

        ui->btnSubirOrden->setEnabled(false);
        ui->btnBajarOrden->setEnabled(false);
    }
}

void Etapas::sortItems()
{
    #ifndef Q_OS_MACOS
        std::sort(etapasDb.begin(), etapasDb.end());
    #endif
}

void Etapas::on_edNombre_textChanged(const QString &arg1)
{
    if (etapasDb[selectedId].nombre != arg1) {
        etapasModel->setData(actualIndex, arg1);
        etapasDb[selectedId].nombre = arg1;
        etapasDb[selectedId].modificado = true;
    }
}

void Etapas::on_edDescripcion_textChanged()
{
    QString controlText = ui->edDescripcion->toPlainText();
    if (controlText != etapasDb[selectedId].descripcion) {
        etapasDb[selectedId].descripcion = controlText;
        etapasDb[selectedId].modificado = true;
    }
}

void Etapas::on_btnBajarOrden_clicked()
{
    changeOrder<EtapaDefinition>(etapasDb, selectedId, true);
    sortItems();
    fillModel();
}

void Etapas::on_btnSubirOrden_clicked()
{
    changeOrder<EtapaDefinition>(etapasDb, selectedId, false);
    sortItems();
    fillModel();
}

void Etapas::on_btnCancelar_clicked()
{
    this->close();
}

/**
 * @brief Etapas::on_btnNuevaEtapa_clicked
 *
 * Pregunta por el nombre de la nueva etapa y la inserta al final de la lista.
 */
void Etapas::on_btnNuevaEtapa_clicked()
{
    bool ok;
    QString nombre = QInputDialog::getText(this, "Etapas", "Indique el nombre de la nueva etapa", QLineEdit::Normal, "", &ok);
    if (ok && !nombre.isEmpty()) {
        EtapaDefinition etapa;
        etapa.nombre = nombre;
        addOrderableElement<EtapaDefinition>(etapasDb, etapa);
        sortItems();
        fillModel();
    }
}

/**
 * @brief Etapas::on_btnEliminar_clicked
 *
 * Elimina una etapa de la lista.
 * La pasa a la lista de etapas eliminadas para cuando se apliquen los cambios.
 */
void Etapas::on_btnEliminar_clicked()
{
    auto [remove, reset] = removeOrderableElement<EtapaDefinition> (etapasDb, selectedId);
    if (remove != nullptr) {
        etapasBorrar << *remove;
    }

    if (reset) {
        fillModel();
        showSelectedItem();
    }
}

/**
 * Guarda los cambios realizados en las etapas.
 * Recorre el array de etapas eliminadas para borrarlas de la base de datos.
 * Si se ha eliminado alguna etapa se entiende que todas las etapas han sido modificadas.
 * En ese caso recorre el array para poner el orden de las etapas.
 * Después recorre el array para modificar o crear las etapas restantes.
 *
 * @brief Etapas::on_btnGuardarEtapa_clicked
 */
void Etapas::on_btnGuardarEtapa_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("¿Quieres guardar los cambios?");
    msgBox.setInformativeText("Si no los guardas perderás los cambios que has realizado");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        QSqlDatabase::database().transaction();

        auto stages = Database::DbElementStage();
        bool actualizarTodos = false;
        if (etapasBorrar.count() > 1) {
            actualizarTodos = true;

            try {
                stages.remove(etapasBorrar);
            } catch (QString msg) {
                QMessageBox msgBox;
                msgBox.setText(msg);
                msgBox.exec();
                return;
            }
        }

        try {
            stages.update(etapasDb, actualizarTodos);
        } catch(QString msg) {
            QMessageBox msgBox;
            msgBox.setText(msg);
            msgBox.exec();
            return;
        }

        QSqlDatabase::database().commit();

        QMessageBox msgBox;
        msgBox.setText("Todos los cambios se han guardado.");
        msgBox.exec();

        close();
    }
}


void Etapas::on_btnSubetapas_clicked()
{
    Subetapas *dlgSubetapas = new Subetapas(etapasDb[selectedId], this);
    dlgSubetapas->exec();
}

void Etapas::on_edCodigo_textChanged(const QString &arg1)
{
    etapasDb[selectedId].codigo = arg1;
    etapasDb[selectedId].modificado = true;
}
