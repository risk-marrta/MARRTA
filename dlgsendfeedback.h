#ifndef DLGSENDFEEDBACK_H
#define DLGSENDFEEDBACK_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>

namespace Ui {
class DlgSendFeedback;
}

class DlgSendFeedback : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSendFeedback(QWidget *parent = nullptr);
    ~DlgSendFeedback();

private slots:
    void on_rbBug_toggled(bool checked);
    void on_btnAddLink_clicked();
    void on_btnRemoveAttachment_clicked();
    void accept() override;
    
    void onIssueCreated(QNetworkReply *reply);

private:
    Ui::DlgSendFeedback *ui;
    QNetworkAccessManager *networkManager;
    QStringList attachmentLinks;
    QString createdIssueUrl;
    
    bool validateForm();
    void submitIssue();
    QString buildIssueBody();
    QString getIssueTitle();
    QStringList getIssueLabels();
    void showIssueLink();
};

#endif // DLGSENDFEEDBACK_H
