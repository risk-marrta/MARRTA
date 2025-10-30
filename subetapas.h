#ifndef SUBETAPAS_H
#define SUBETAPAS_H

#include <QDialog>

#include "etapas.h"
#include "orderable.h"
namespace Ui {
class Subetapas;
}

struct SubetapaDefinition: OrderableDefinition {
    bool activo;
    int etapaId;
};

class Subetapas : public QDialog
{
    Q_OBJECT

public:
    explicit Subetapas(EtapaDefinition &etapa, QWidget *parent = nullptr);
    ~Subetapas();

private slots:
    void on_btnCancelar_clicked();

    void on_btnNueva_clicked();

    void on_btnEliminar_clicked();

    void on_btnGuardar_clicked();

    void on_listaSubetapas_customContextMenuRequested(const QPoint &pos);

    void on_btnSubirOrden_clicked();

    void on_btnBajarOrden_clicked();

private:
    Ui::Subetapas *ui;

    EtapaDefinition etapa;

    QList<SubetapaDefinition> subetapasDb;
    QList<SubetapaDefinition> subetapasBorrar;
    int selectedId;
    QModelIndex actualIndex;

    QStandardItemModel *subetapasModel = nullptr;

    void fillModel();
    void showSelectedItem();
    void sortItems();
};

#endif // SUBETAPAS_H
