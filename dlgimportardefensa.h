#ifndef DLGIMPORTARDEFENSA_H
#define DLGIMPORTARDEFENSA_H

#include <QDialog>
#include <QStringListModel>

#include "dbmanager.h"

namespace Ui {
class DlgImportarDefensa;
}

class DlgImportarDefensa : public QDialog
{
    Q_OBJECT

public:
    explicit DlgImportarDefensa(QWidget *parent = nullptr);
    explicit DlgImportarDefensa(QWidget *parent = nullptr, DefenseType _defense = DefenseType::FrecuencyReducer, QStandardItemModel *currentModel = nullptr);
    ~DlgImportarDefensa();

    DefenseDefinition getSelected() {
        return selected;
    }

private slots:
    void on_btCancelar_clicked();

    //void on_listDefenses_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_btAceptar_clicked();

    //void on_listDefenses_itemDoubleClicked(QListWidgetItem *item);

    void on_listDefenses_clicked(const QModelIndex &index);

    void on_listDefenses_activated(const QModelIndex &index);

    void on_listDefenses_doubleClicked(const QModelIndex &index);

    void on_edFilter_textChanged(const QString &arg1);

    void on_DlgImportarDefensa_rejected();

    void on_DlgImportarDefensa_finished(int result);

private:
    Ui::DlgImportarDefensa *ui;
    QList<DefenseDefinition> defensas;
    DefenseDefinition selected;
    DefenseType defense;
    QStringListModel model;
    QStandardItemModel *actualModel;

    QString actualFilter;

    void listarDefensas();
    void seleccionarDefensa(const QModelIndex &index);
};

#endif // DLGIMPORTARDEFENSA_H
