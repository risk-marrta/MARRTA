#include "dlgerror.h"
#include "ui_dlgerror.h"

DlgError::DlgError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgError)
{
    ui->setupUi(this);
}

DlgError::~DlgError()
{
    delete ui;
}

void DlgError::on_buttonBox_accepted()
{
    close();
}

void DlgError::addText(QString text)
{
    QString actualText = ui->errorsText->text();
    actualText.append(text);
    ui->errorsText->setText(actualText);
}
