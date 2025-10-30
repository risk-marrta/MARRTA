#include "dlgsendfeedback.h"
#include "ui_dlgsendfeedback.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QInputDialog>

// GitHub API endpoint for creating issues
#define GITHUB_API_CREATE_ISSUE "https://api.github.com/repos/risk-marrta/MARRTA/issues"

DlgSendFeedback::DlgSendFeedback(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSendFeedback),
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    
    // Initially hide bug details group
    ui->groupBugDetails->setVisible(ui->rbBug->isChecked());
    
    // Connect to monitor comment type changes
    connect(ui->rbBug, &QRadioButton::toggled, this, &DlgSendFeedback::on_rbBug_toggled);
}

DlgSendFeedback::~DlgSendFeedback()
{
    delete ui;
}

void DlgSendFeedback::on_rbBug_toggled(bool checked)
{
    // Show/hide bug details group based on whether bug is selected
    ui->groupBugDetails->setVisible(checked);
}

void DlgSendFeedback::on_btnAddLink_clicked()
{
    bool ok;
    QString link = QInputDialog::getText(
        this,
        tr("Añadir enlace a archivo"),
        tr("Ingrese el enlace al archivo hospedado\n(Google Drive, OneDrive, iCloud, Dropbox, etc.):"),
        QLineEdit::Normal,
        QString(),
        &ok
    );
    
    if (ok && !link.trimmed().isEmpty()) {
        if (!attachmentLinks.contains(link.trimmed())) {
            attachmentLinks.append(link.trimmed());
            ui->listAttachments->addItem(link.trimmed());
        }
    }
}

void DlgSendFeedback::on_btnRemoveAttachment_clicked()
{
    int currentRow = ui->listAttachments->currentRow();
    if (currentRow >= 0) {
        attachmentLinks.removeAt(currentRow);
        delete ui->listAttachments->takeItem(currentRow);
    }
}

bool DlgSendFeedback::validateForm()
{
    // Validate required fields
    if (ui->edName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Por favor, ingrese su nombre."));
        ui->edName->setFocus();
        return false;
    }
    
    if (ui->edEmail->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Por favor, ingrese su email de contacto."));
        ui->edEmail->setFocus();
        return false;
    }
    
    // Basic email validation
    QString email = ui->edEmail->text().trimmed();
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, tr("Error"), tr("Por favor, ingrese un email válido."));
        ui->edEmail->setFocus();
        return false;
    }
    
    if (ui->edDescription->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Por favor, ingrese una descripción del asunto."));
        ui->edDescription->setFocus();
        return false;
    }
    
    // Validate bug-specific fields if bug is selected
    if (ui->rbBug->isChecked()) {
        if (ui->edSteps->toPlainText().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("Por favor, ingrese los pasos para reproducir el error."));
            ui->edSteps->setFocus();
            return false;
        }
        
        if (ui->edExpected->toPlainText().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("Por favor, ingrese el resultado esperado."));
            ui->edExpected->setFocus();
            return false;
        }
        
        if (ui->edActual->toPlainText().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("Por favor, ingrese el resultado obtenido."));
            ui->edActual->setFocus();
            return false;
        }
    }
    
    return true;
}

QString DlgSendFeedback::getIssueTitle()
{
    QString prefix;
    if (ui->rbBug->isChecked()) {
        prefix = tr("[Bug]");
    } else if (ui->rbFeature->isChecked()) {
        prefix = tr("[Feature Request]");
    } else {
        prefix = tr("[Comentario]");
    }
    
    // Get first line of description as title (max 100 chars)
    QString description = ui->edDescription->toPlainText().trimmed();
    QString firstLine = description.split('\n').first();
    if (firstLine.length() > 100) {
        firstLine = firstLine.left(97) + "...";
    }
    
    return QString("%1 %2").arg(prefix, firstLine);
}

QStringList DlgSendFeedback::getIssueLabels()
{
    QStringList labels;
    
    if (ui->rbBug->isChecked()) {
        labels.append("bug");
    } else if (ui->rbFeature->isChecked()) {
        labels.append("enhancement");
    } else {
        labels.append("question");
    }
    
    // Add user-submitted label to distinguish from internal issues
    labels.append("user-submitted");
    
    return labels;
}

QString DlgSendFeedback::buildIssueBody()
{
    QString body;
    
    // Add submitter information
    body += tr("## Información del remitente\n\n");
    body += tr("**Nombre:** %1\n").arg(ui->edName->text().trimmed());
    if (!ui->edInstitution->text().trimmed().isEmpty()) {
        body += tr("**Institución:** %1\n").arg(ui->edInstitution->text().trimmed());
    }
    body += tr("**Email de contacto:** %1\n").arg(ui->edEmail->text().trimmed());
    body += tr("**Versión de MARRTA:** %1\n\n").arg(APP_VERSION);
    
    // Add description
    body += tr("## Descripción\n\n");
    body += ui->edDescription->toPlainText().trimmed() + "\n\n";
    
    // Add bug-specific details if applicable
    if (ui->rbBug->isChecked()) {
        body += tr("## Detalles del error\n\n");
        body += tr("### Pasos para reproducir\n\n");
        body += ui->edSteps->toPlainText().trimmed() + "\n\n";
        
        body += tr("### Resultado esperado\n\n");
        body += ui->edExpected->toPlainText().trimmed() + "\n\n";
        
        body += tr("### Resultado obtenido\n\n");
        body += ui->edActual->toPlainText().trimmed() + "\n\n";
    }
    
    // Add links to attachments if any
    if (!attachmentLinks.isEmpty()) {
        body += tr("## Enlaces a archivos adjuntos\n\n");
        body += tr("El usuario ha proporcionado los siguientes enlaces a archivos:\n");
        for (const QString &link : attachmentLinks) {
            body += QString("- %1\n").arg(link);
        }
        body += "\n";
    }
    
    // Add disclaimer
    body += tr("\n---\n");
    body += tr("*Este issue fue creado automáticamente desde la aplicación MARRTA.*\n");
    
    return body;
}

void DlgSendFeedback::accept()
{
    if (!validateForm()) {
        return;
    }
    
    // Show information and confirmation dialog
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Información importante"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("La información remitida por este medio estará visible a otros usuarios y públicamente en https://github.com/risk-marrta/MARRTA."));
    msgBox.setInformativeText(tr("¿Desea continuar y remitir su comentario?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    
    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }
    
    submitIssue();
}

void DlgSendFeedback::submitIssue()
{
    // Disable form while submitting
    ui->buttonBox->setEnabled(false);
    
    // Build JSON payload
    QJsonObject json;
    json["title"] = getIssueTitle();
    json["body"] = buildIssueBody();
    
    QJsonArray labelsArray;
    for (const QString &label : getIssueLabels()) {
        labelsArray.append(label);
    }
    json["labels"] = labelsArray;
    
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    // Create network request
    QUrl url(GITHUB_API_CREATE_ISSUE);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader, "MARRTA-FeedbackSubmitter");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");
    
    // Note: For public repositories, creating issues doesn't require authentication
    // GitHub allows anonymous issue creation via the web interface, but the API
    // requires authentication. We'll need a GitHub token or use a different approach.
    // For now, we'll try without authentication and handle the error appropriately.
    
    qDebug() << "Submitting issue to GitHub...";
    qDebug() << "Request data:" << data;
    
    // Connect reply handler
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &DlgSendFeedback::onIssueCreated);
    
    // Send POST request
    networkManager->post(request, data);
}

void DlgSendFeedback::onIssueCreated(QNetworkReply *reply)
{
    // Disconnect to avoid multiple calls
    disconnect(networkManager, &QNetworkAccessManager::finished,
               this, &DlgSendFeedback::onIssueCreated);
    
    reply->deleteLater();
    
    // Re-enable form
    ui->buttonBox->setEnabled(true);
    
    // Check for errors
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error creating issue:" << reply->errorString();
        qWarning() << "Response:" << reply->readAll();
        
        // Check if it's an authentication error
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (httpStatus == 401 || httpStatus == 403) {
            QMessageBox::critical(
                this,
                tr("Error de autenticación"),
                tr("No se pudo crear el issue automáticamente porque GitHub requiere autenticación.\n\n"
                   "Por favor, cree el issue manualmente visitando:\n"
                   "https://github.com/risk-marrta/MARRTA/issues/new\n\n"
                   "O envíe su comentario por email a: matrices.marrta@gmail.com\n\n"
                   "Puede copiar el contenido de este formulario y pegarlo en el issue o email.")
            );
            
            // Show a dialog with the issue content for manual submission
            QString issueContent = tr("Título: %1\n\n").arg(getIssueTitle());
            issueContent += buildIssueBody();
            
            QMessageBox contentBox(this);
            contentBox.setWindowTitle(tr("Contenido del issue"));
            contentBox.setText(tr("Copie el siguiente contenido para crear el issue manualmente:"));
            contentBox.setDetailedText(issueContent);
            contentBox.setIcon(QMessageBox::Information);
            contentBox.exec();
        } else {
            QMessageBox::critical(
                this,
                tr("Error"),
                tr("No se pudo enviar el comentario: %1\n\n"
                   "Por favor, intente nuevamente más tarde, cree el issue manualmente en:\n"
                   "https://github.com/risk-marrta/MARRTA/issues/new\n\n"
                   "O envíe su comentario por email a: matrices.marrta@gmail.com").arg(reply->errorString())
            );
        }
        return;
    }
    
    // Parse response to get issue URL
    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonResponse = doc.object();
    
    createdIssueUrl = jsonResponse["html_url"].toString();
    QString issueNumber = QString::number(jsonResponse["number"].toInt());
    
    qDebug() << "Issue created successfully:" << createdIssueUrl;
    
    // Show success message with link
    showIssueLink();
    
    // Close dialog
    QDialog::accept();
}

void DlgSendFeedback::showIssueLink()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Comentario enviado"));
    msgBox.setText(tr("Su comentario ha sido enviado exitosamente."));
    msgBox.setInformativeText(tr("Puede ver y seguir el progreso en:\n%1\n\n"
                                  "¿Desea abrir el issue en su navegador?").arg(createdIssueUrl));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    
    if (msgBox.exec() == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl(createdIssueUrl));
    }
}
