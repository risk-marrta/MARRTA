#-------------------------------------------------
#
# Project created by QtCreator 2019-06-08T18:58:03
#
#-------------------------------------------------

QT       += core gui-private qml sql widgets charts printsupport network webenginewidgets

TARGET = marrta
TEMPLATE = app

PYTHON_ENABLED = 1

RC_ICONS = logo.ico

# Pide C++17 de forma portable
CONFIG += c++17

# Flags específicos por compilador:
gcc|clang {
    QMAKE_CXXFLAGS += -std=c++17 -fno-sized-deallocation
}

msvc {
    # Equivalente MSVC (si de verdad necesitas tocar el estándar)
    QMAKE_CXXFLAGS += /std:c++17
    # Equivalente a -fno-sized-deallocation en MSVC:
    # (solo si realmente necesitas desactivar sized delete)
    QMAKE_CXXFLAGS += /Zc:sizedDealloc-
    # Opcional: silenciar el warning C4267 (size_t -> int) de QXlsx
    QMAKE_CXXFLAGS_WARN_ON += /wd4267
}

unix: QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"
unix: QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

# Python integration (optional)
# To enable Python scripting, set PYTHON_ENABLED=1 and configure paths below
# Example: qmake "PYTHON_ENABLED=1" "PYTHON_INCLUDE=C:/Python312/include" "PYTHON_LIB=C:/Python312/libs"

contains(PYTHON_ENABLED, 1) {
    message("Python scripting support enabled")
    DEFINES += PYTHON_ENABLED
    
    # Workaround for Qt/Python slots conflict on Windows
    win32 {
        DEFINES += HAVE_ROUND
    }
    
    unix {
        isEmpty(PYTHON_INCLUDE) {
            PYTHON_INCLUDE = /usr/include/python3.12
        }
        isEmpty(PYTHON_LIB_NAME) {
            PYTHON_LIB_NAME = python3.12
        }
        INCLUDEPATH += $$PYTHON_INCLUDE
        LIBS += -l$$PYTHON_LIB_NAME
    }
    
    win32 {
        # Windows: Set PYTHON_INCLUDE and PYTHON_LIB paths
        # Default assumes standard Python 3.12 installation
        isEmpty(PYTHON_INCLUDE) {
            PYTHON_INCLUDE = "C:/Program Files/Python312/include"
        }
        isEmpty(PYTHON_LIB) {
            PYTHON_LIB = "C:/Program Files/Python312/libs"
        }
        isEmpty(PYTHON_LIB_NAME) {
            PYTHON_LIB_NAME = python312
        }
        INCLUDEPATH += $$PYTHON_INCLUDE
        LIBS += -L$$PYTHON_LIB -l$$PYTHON_LIB_NAME
    }
} else {
    message("Python scripting support disabled (use PYTHON_ENABLED=1 to enable)")
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Get git commit hash for build number
GIT_COMMIT = $$system(git log -1 --format=%h)
isEmpty(GIT_COMMIT) {
    GIT_COMMIT = "unknown"
}
DEFINES += GIT_COMMIT=\\\"$$GIT_COMMIT\\\"

QXLSX_PARENTPATH=./QXlsx/         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=$${QXLSX_PARENTPATH}header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=$${QXLSX_PARENTPATH}source/  # current QXlsx source path is ./source/
include($${QXLSX_PARENTPATH}QXlsx.pri)

CONFIG += c++17

SOURCES += \
    about.cpp \
    adapters/chartadapter.cpp \
    adapters/consoleadapter.cpp \
    adapters/defenseadapter.cpp \
    adapters/eventadapter.cpp \
    adapters/stageadapter.cpp \
    calculodefensa.cpp \
    updatechecker.cpp \
    completeeventdefinition.cpp \
    dbdefense.cpp \
    dbelement.cpp \
    dbelementstage.cpp \
    dbelementsubstage.cpp \
    dbevent.cpp \
    dbexception.cpp \
    dbmanager.cpp \
    defensas.cpp \
    defensecalc.cpp \
    defensedefinition.cpp \
    defenselistmodel.cpp \
    defenseitemdelegate.cpp \
    dlgadddefensetoevents.cpp \
    dlgconfiginforme.cpp \
    dlgcomparadormodelos.cpp \
    dlgdisclaimer.cpp \
    dlgerror.cpp \
    dlgimportardefensa.cpp \
    dlglicense.cpp \
    dlgsendfeedback.cpp \
    eventdefinition.cpp \
    eventhomepage.cpp \
    flujogramagenerator.cpp \
    importwizard.cpp \
    informacionmodelo.cpp \
    informegenerator.cpp \
    javascript_highlighter.cpp \
    python_highlighter.cpp \
    pythonscriptingengine.cpp \
    pythonscriptingeditor.cpp \
    pythonscriptingdocumentation.cpp \
    listabarreras.cpp \
    htmlpreviewhelper.cpp \
    main.cpp \
    mainwindow.cpp \
    newmodelwizard.cpp \
    etapas.cpp \
    dlggestionetapas.cpp \
    matrizriesgo.cpp \
    modelos/empty.cpp \
    modelos/importsevrra.cpp \
    modelos/marr.cpp \
    modelos/modelo.cpp \
    modelos/xlsx.cpp \
    resumen.cpp \
    scriptingdocumentation.cpp \
    scriptingeditor.cpp \
    scriptingengine.cpp \
    subetapas.cpp \
    sucesos.cpp \
    switchbutton.cpp \
    usermanualdocumentation.cpp \
    waitingspinnerwidget.cpp \
    widget/completedelegate.cpp \
    widget/orientablepushbutton.cpp

HEADERS += \
    about.h \
    adapters/chartadapter.h \
    adapters/consoleadapter.h \
    adapters/defenseadapter.h \
    adapters/eventadapter.h \
    adapters/stageadapter.h \
    calculodefensa.h \
    updatechecker.h \
    completeeventdefinition.h \
    dbdefense.h \
    dbelement.h \
    dbelementstage.h \
    dbelementsubstage.h \
    dbevent.h \
    dbexception.h \
    dbmanager.h \
    defensas.h \
    defensecalc.h \
    defensedefinition.h \
    defenselistmodel.h \
    defenseitemdelegate.h \
    dlgadddefensetoevents.h \
    dlgconfiginforme.h \
    dlgcomparadormodelos.h \
    dlgdisclaimer.h \
    dlgerror.h \
    dlgimportardefensa.h \
    dlglicense.h \
    dlgsendfeedback.h \
    eventdefinition.h \
    eventhomepage.h \
    flujogramagenerator.h \
    importwizard.h \
    informacionmodelo.h \
    informegenerator.h \
    javascript_highlighter.h \
    python_highlighter.h \
    pythonscriptingengine.h \
    pythonscriptingeditor.h \
    pythonscriptingdocumentation.h \
    listabarreras.h \
    htmlpreviewhelper.h \
    mainwindow.h \
    newmodelwizard.h \
    etapas.h \
    dlggestionetapas.h \
    matrizriesgo.h \
    modelos/empty.h \
    modelos/importsevrra.h \
    modelos/marr.h \
    modelos/modelo.h \
    modelos/xlsx.h \
    orderable.h \
    orderablewidget.h \
    resumen.h \
    scriptingdocumentation.h \
    scriptingeditor.h \
    scriptingengine.h \
    settings.h \
    style.h \
    styles.h \
    subetapas.h \
    sucesos.h \
    switchbutton.h \
    usermanualdocumentation.h \
    waitingspinnerwidget.h \
    widget/completedelegate.h \
    widget/orientablepushbutton.h

FORMS += \
    about.ui \
    calculodefensa.ui \
    defensas.ui \
    dlgadddefensetoevents.ui \
    dlgconfiginforme.ui \
    dlgcomparadormodelos.ui \
    dlgdisclaimer.ui \
    dlgerror.ui \
    dlgimportardefensa.ui \
    dlglicense.ui \
    dlgsendfeedback.ui \
    importwizard.ui \
    informacionmodelo.ui \
    listabarreras.ui \
        mainwindow.ui \
    newmodelwizard.ui \
    etapas.ui \
    dlggestionetapas.ui \
    resumen.ui \
    scriptingeditor.ui \
    pythonscriptingeditor.ui \
    subetapas.ui \
    sucesos.ui

TRANSLATIONS = marrta_es.ts \
    marrta_en.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix: CONFIG += link_pkgconfig

RESOURCES += \
    icons.qrc \
    logos.qrc \
    qdarkstyle/style.qrc \
    translations.qrc

DISTFILES +=

