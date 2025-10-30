#ifndef ETAPAS_H
#define ETAPAS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QStringListModel>
#include <QListWidgetItem>
#include <QStandardItemModel>

#include "orderable.h"
#include "orderablewidget.h"

namespace Ui {
class Etapas;
}

struct EtapaDefinition: OrderableDefinition {
    QStringList subetapas;
    QString codigo;
};

class Etapas : public QDialog
{
    Q_OBJECT

public:
    explicit Etapas(QWidget *parent = nullptr);
    ~Etapas();

public slots:


private slots:

    void on_edNombre_textChanged(const QString &arg1);

    void on_btnBajarOrden_clicked();

    void on_btnSubirOrden_clicked();

    void on_btnCancelar_clicked();

    void on_btnNuevaEtapa_clicked();

    void on_btnEliminar_clicked();

    void on_btnGuardarEtapa_clicked();

    void on_edDescripcion_textChanged();

    void on_btnSubetapas_clicked();

    void on_edCodigo_textChanged(const QString &arg1);

private:
    Ui::Etapas *ui;
    QSqlDatabase *praxisDb;
    QStandardItemModel *etapasModel;

    QList<EtapaDefinition> etapasDb;
    QList<EtapaDefinition> etapasBorrar;
    int selectedId;
    QModelIndex actualIndex;

    void fillModel();
    void showSelectedItem();
    void sortItems();
};

#endif // ETAPAS_H
