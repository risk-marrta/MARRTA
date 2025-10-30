#ifndef DLGLICENSE_H
#define DLGLICENSE_H

#include <QDialog>

namespace Ui {
class DlgLicense;
}

class DlgLicense : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLicense(QWidget *parent = nullptr);
    ~DlgLicense();

private slots:
    void on_btnLicenseLink_clicked();

private:
    Ui::DlgLicense *ui;
    QString getCurrentLanguage();
    QString getLicenseUrl(const QString& language);
};

#endif // DLGLICENSE_H
