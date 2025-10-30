#ifndef DLGDISCLAIMER_H
#define DLGDISCLAIMER_H

#include <QDialog>

namespace Ui {
class DlgDisclaimer;
}

class DlgDisclaimer : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDisclaimer(QWidget *parent = nullptr);
    ~DlgDisclaimer();

private:
    Ui::DlgDisclaimer *ui;
};

#endif // DLGDISCLAIMER_H
