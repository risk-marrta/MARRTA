#include "htmlpreviewhelper.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QCoreApplication>
#include <QEventLoop>
#include <QPointer>
#include <QStandardPaths>
#include <QDir>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QWebEngineView>
    #include <QWebEnginePage>
#else
    #include <QtWebEngineWidgets/QWebEngineView>
    #include <QtWebEngineWidgets/QWebEnginePage>
#endif

HtmlPreviewHelper &HtmlPreviewHelper::instance()
{
    static HtmlPreviewHelper helper;
    return helper;
}

HtmlPreviewHelper::HtmlPreviewHelper(QObject *parent)
    : QObject(parent),
      previewDialog(new QDialog()),
      previewView(new QWebEngineView(previewDialog)),
      devToolsDialog(nullptr),
      devToolsView(nullptr),
      inspectButton(new QPushButton()),
      saveButton(new QPushButton()),
      saveStylesButton(new QPushButton()),
      importStylesButton(new QPushButton()),
      resetStylesButton(new QPushButton()),
      closeButton(new QPushButton()),
      currentParent(nullptr)
{
    initializeUi();
}

void HtmlPreviewHelper::initializeUi()
{
    previewDialog->setWindowTitle(tr("Vista Previa del Informe HTML"));
    previewDialog->resize(900, 700);
    previewDialog->setModal(true);

    inspectButton->setParent(previewDialog);
    saveButton->setParent(previewDialog);
    saveStylesButton->setParent(previewDialog);
    importStylesButton->setParent(previewDialog);
    resetStylesButton->setParent(previewDialog);
    closeButton->setParent(previewDialog);
    
    inspectButton->setText(tr("Inspeccionar estilos"));
    saveButton->setText(tr("Guardar HTML"));
    saveStylesButton->setText(tr("Guardar estilos"));
    importStylesButton->setText(tr("Importar estilos"));
    resetStylesButton->setText(tr("Restaurar"));
    closeButton->setText(tr("Cerrar"));

    QVBoxLayout *layout = new QVBoxLayout(previewDialog);
    layout->addWidget(previewView);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(inspectButton);
    buttonLayout->addWidget(saveStylesButton);
    buttonLayout->addWidget(importStylesButton);
    buttonLayout->addWidget(resetStylesButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, previewDialog, &QDialog::accept);

    connect(inspectButton, &QPushButton::clicked, this, [this]() {
        ensureDevTools();
        if (devToolsDialog) {
            devToolsDialog->show();
            devToolsDialog->raise();
            devToolsDialog->activateWindow();
        }
    });

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        const QString fileName = QFileDialog::getSaveFileName(previewDialog,
            tr("Guardar HTML"),
            QString(),
            tr("Archivos HTML (*.html)"));

        if (fileName.isEmpty()) {
            return;
        }

        QString target = fileName;
        if (!target.endsWith(".html", Qt::CaseInsensitive)) {
            target += ".html";
        }

        QFile file(target);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << currentHtml;
            file.close();
            QMessageBox::information(previewDialog,
                                     tr("Éxito"),
                                     tr("El archivo HTML se ha guardado correctamente en:\n%1").arg(target));
        } else {
            QMessageBox::critical(previewDialog,
                                  tr("Error"),
                                  tr("No se pudo guardar el archivo HTML"));
        }
    });
    
    connect(saveStylesButton, &QPushButton::clicked, this, &HtmlPreviewHelper::saveCurrentCSS);
    connect(importStylesButton, &QPushButton::clicked, this, &HtmlPreviewHelper::importCSS);
    connect(resetStylesButton, &QPushButton::clicked, this, &HtmlPreviewHelper::resetCSS);
}

void HtmlPreviewHelper::showPreview(const QString &html, QWidget *parent)
{
    currentHtml = html;
    currentParent = parent;

    previewDialog->setWindowModality(Qt::WindowModal);
    previewView->setHtml(html);

    previewDialog->exec();

    previewView->page()->triggerAction(QWebEnginePage::Stop);
    if (devToolsDialog) {
        devToolsDialog->hide();
    }

    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    restoreFocus();
}

void HtmlPreviewHelper::ensureDevTools()
{
    if (!devToolsDialog) {
        devToolsDialog = new QDialog(previewDialog);
        devToolsDialog->setWindowTitle(tr("Herramientas de desarrollo"));
        devToolsDialog->resize(1200, 800);

        QVBoxLayout *layout = new QVBoxLayout(devToolsDialog);
        layout->setContentsMargins(0, 0, 0, 0);

        devToolsView = new QWebEngineView(devToolsDialog);
        layout->addWidget(devToolsView);

        previewView->page()->setDevToolsPage(devToolsView->page());
    } else if (devToolsDialog->parentWidget() != previewDialog) {
        devToolsDialog->setParent(previewDialog);
    }
}

void HtmlPreviewHelper::restoreFocus()
{
    if (!currentParent) {
        return;
    }

    QWidget *dialogWindow = currentParent->isWindow() ? currentParent : currentParent->window();
    QWidget *mainWindow = dialogWindow && dialogWindow->parentWidget() ? dialogWindow->parentWidget()->window() : nullptr;

    QPointer<QWidget> safeDialog(dialogWindow);
    QPointer<QWidget> safeMain(mainWindow);

    QTimer::singleShot(0, previewDialog, [safeDialog, safeMain]() {
        if (safeDialog) {
            safeDialog->raise();
            safeDialog->activateWindow();
            safeDialog->setFocus(Qt::ActiveWindowFocusReason);
        }
        if (safeMain) {
            safeMain->raise();
            safeMain->activateWindow();
        }
    });
}

void HtmlPreviewHelper::saveCurrentCSS()
{
    // Get user CSS path
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString userCSSPath = dataPath + "/report_style.css";
    
    // Try to load user's custom CSS first, otherwise get default
    QString currentCSS;
    QFile userFile(userCSSPath);
    if (userFile.exists() && userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&userFile);
        currentCSS = in.readAll();
        userFile.close();
    }
    
    // If no user CSS, load default from resources
    if (currentCSS.isEmpty()) {
        QFile file(":/report/resources/report_style.css");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            currentCSS = in.readAll();
        }
    }
    
    // Ask user where to save
    QString fileName = QFileDialog::getSaveFileName(previewDialog,
        tr("Guardar Estilos de Informe"),
        QString(),
        tr("Archivos CSS (*.css)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    if (!fileName.endsWith(".css", Qt::CaseInsensitive)) {
        fileName += ".css";
    }
    
    // Save to file
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << currentCSS;
        file.close();
        QMessageBox::information(previewDialog, tr("Éxito"),
            tr("Los estilos se han guardado correctamente en:\n%1").arg(fileName));
    } else {
        QMessageBox::critical(previewDialog, tr("Error"),
            tr("No se pudo guardar el archivo de estilos"));
    }
}

void HtmlPreviewHelper::importCSS()
{
    // Ask user to select CSS file
    QString fileName = QFileDialog::getOpenFileName(previewDialog,
        tr("Importar Estilos de Informe"),
        QString(),
        tr("Archivos CSS (*.css)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Load CSS from file
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString css = in.readAll();
        file.close();
        
        // Save as user CSS
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir(dataPath);
        if (!dir.exists()) {
            dir.mkpath(dataPath);
        }
        QString userCSSPath = dataPath + "/report_style.css";
        
        QFile outFile(userCSSPath);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outFile);
            out << css;
            outFile.close();
            
            QMessageBox::information(previewDialog, tr("Éxito"),
                tr("Los estilos se han importado correctamente.\n\nPara continuar, se cerrará esta previsualización, y al volver a generarla se aplicarán los estilos seleccionados."));
            
            // Close the preview dialog
            previewDialog->accept();
        } else {
            QMessageBox::critical(previewDialog, tr("Error"),
                tr("No se pudo guardar los estilos importados"));
        }
    } else {
        QMessageBox::critical(previewDialog, tr("Error"),
            tr("No se pudo leer el archivo de estilos"));
    }
}

void HtmlPreviewHelper::resetCSS()
{
    // Confirm action with custom buttons
    QMessageBox msgBox(previewDialog);
    msgBox.setWindowTitle(tr("Restaurar estilos por defecto"));
    msgBox.setText(tr("¿Está seguro que desea restaurar los estilos por defecto?\nSe perderán las personalizaciones actuales."));
    msgBox.setIcon(QMessageBox::Question);
    
    QPushButton *yesButton = msgBox.addButton(tr("Sí"), QMessageBox::YesRole);
    QPushButton *noButton = msgBox.addButton(tr("No"), QMessageBox::NoRole);
    msgBox.setDefaultButton(noButton);
    
    msgBox.exec();
    
    if (msgBox.clickedButton() == yesButton) {
        // Delete user CSS file
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QString userCSSPath = dataPath + "/report_style.css";
        QFile::remove(userCSSPath);
        
        QMessageBox::information(previewDialog, tr("Éxito"),
            tr("Los estilos se han restaurado al estilo por defecto.\n\nPara continuar, se cerrará esta previsualización, y al volver a generarla se aplicarán los estilos seleccionados."));
        
        // Close the preview dialog
        previewDialog->accept();
    }
}
