#ifndef DEFENSAS_H
#define DEFENSAS_H

#include <QDialog>
#include <QStandardItemModel>
#include "defensedefinition.h"
#include "orderablewidget.h"
#include "dbelementstage.h"

namespace Ui {
class Defensas;
}

class Defensas : public QDialog
{
    Q_OBJECT

public:
    explicit Defensas(QWidget *parent = nullptr, int _eventId = -1, DefenseType _defense = DefenseType::FrecuencyReducer);
    ~Defensas();

    void cambiarTitulo(QString tipo, QString suceso);
    void establecerEvento(int _eventId, DefenseType _defense);

private slots:
    void on_btnNewDefense_clicked();

    void on_edName_textChanged(const QString &arg1);

    void on_cbHardiness_currentIndexChanged(int index);

    void on_btnRemoveDefense_clicked();

    void on_btnCancel_clicked();

    void on_btnSave_clicked();

    void on_defenseList_customContextMenuRequested(const QPoint &pos);

    void on_edDescripcion_textChanged();

    void on_cbEtapasOrden_currentIndexChanged(int index);

    void on_toolButton_clicked();

    void on_edCode_textChanged(const QString &arg1);

    void on_btRecargarCodigo_clicked();

private:
    Ui::Defensas *ui;
    int eventId;
    DefenseType defense;

    QList<DefenseDefinition> defensasDb;
    QList<DefenseDefinition> defensasBorrar;
    QStandardItemModel *defensasModel;
    // Etapa original del evento
    int etapaOrdenInicial;
    int etapaOrdenValorInicial;
    QString codigoInicial;
    // Etapas con el próximo valor del orden
    std::map<int, Database::Orders> etapasOrden;
    int selectedId = -1;
    QModelIndex actualIndex;

private:
    void cargarDefensas();

    void fillModel();
    void listOrderStages();
    void showSelectedItem();
    void sortItems();

    /**
     * Devuelve la primera parte del código de una defensa
     * según su tipo.
     */
    inline QString primeraParteCodigoDefensa();
    /**
     * @brief comprobarCodigosActuales comprueba si en la lista de defensas actuales hay algún número mayor a usar
     * @return
     */
    int comprobarCodigosActuales(QString, int);
    
    /**
     * @brief recalcularRiesgoEvento Recalcula el riesgo de un evento basado en sus defensas
     * @param eventoId ID del evento
     */
    void recalcularRiesgoEvento(int eventoId);
};

#endif // DEFENSAS_H
