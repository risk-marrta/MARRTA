#ifndef DLGERROR_H
#define DLGERROR_H

#include <QDialog>

namespace Ui {
class DlgError;
}

class DlgError : public QDialog
{
    Q_OBJECT

public:
    explicit DlgError(QWidget *parent = nullptr);
    ~DlgError();

    void addText(QString text);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgError *ui;
};

#endif // DLGERROR_H
