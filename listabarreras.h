#ifndef LISTABARRERAS_H
#define LISTABARRERAS_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QSortFilterProxyModel>
#include "defensedefinition.h"

namespace Ui {
class ListaBarreras;
}

// Proxy model para ordenación numérica personalizada
class NumericSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit NumericSortProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        // Para columnas 1-5, usar UserRole si está disponible para ordenar numéricamente
        int column = left.column();
        if (column >= 1 && column <= 5) {
            QVariant leftData = sourceModel()->data(left, Qt::UserRole);
            QVariant rightData = sourceModel()->data(right, Qt::UserRole);
            
            if (leftData.isValid() && rightData.isValid()) {
                // Ordenar numéricamente usando UserRole
                return leftData.toInt() < rightData.toInt();
            }
        }
        
        // Para otras columnas, usar ordenación por defecto (texto)
        return QSortFilterProxyModel::lessThan(left, right);
    }
};

// Delegate para mostrar combobox en columna de robustez
class RobustezDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RobustezDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

// Estructura que representa la información de un evento asociado a una defensa.
struct EventoInfo {
    int eventoId;           // Identificador único del evento
    int defensaId;          // Identificador de la defensa asociada
    QString codigo;         // Código del evento
    QString nombre;         // Nombre del evento
    QString descripcion;    // Descripción del evento
    int frecuencia;         // Frecuencia actual del evento (valor calculado o modificado)
    int consecuencia;       // Consecuencia actual del evento (valor calculado o modificado)
    int frecuenciaBase;     // Frecuencia base del evento (valor original antes de aplicar defensas)
    int consecuenciaBase;   // Consecuencia base del evento (valor original antes de aplicar defensas)
    int riesgo;             // Riesgo actual del evento (calculado como frecuencia * consecuencia)
    int riesgoBase;         // Riesgo base del evento (calculado como frecuenciaBase * consecuenciaBase)
    bool activa;            // Indica si la defensa está activa para este evento
};

class ListaBarreras : public QDialog
{
    Q_OBJECT

public:
    explicit ListaBarreras(QWidget *parent = nullptr);
    ~ListaBarreras();

signals:
    void defensesChanged(); // Signal emitido cuando se cambian defensas

private slots:
    void on_btCerrar_clicked();
    void on_edFiltro_textChanged(const QString &arg1);
    void onItemChanged(QStandardItem *item);
    void onItemDoubleClicked(const QModelIndex &index);
    void showContextMenu(const QPoint &pos);

private:
    Ui::ListaBarreras *ui;
    QStandardItemModel *modelReductoresFrecuencia;
    QStandardItemModel *modelBarreras;
    QStandardItemModel *modelReductoresConsecuencia;
    NumericSortProxyModel *proxyReductoresFrecuencia;
    NumericSortProxyModel *proxyBarreras;
    NumericSortProxyModel *proxyReductoresConsecuencia;
    QList<DefenseDescription> defensasUnicas;
    
    void cargarDefensas();
    void cargarDefensasConEventos(QStandardItemModel *model, DefenseType tipo);
    void aplicarFiltro();
    QList<EventoInfo> obtenerEventosPorDefensa(const QString &nombreDefensa, DefenseType tipo);
    void toggleDefenseForEvent(int defensaId, int eventoId, bool activar);
    void desactivarDefensaEnTodosEventos(const QString &nombreDefensa, int tipoDefensa);
    void activarDefensaEnTodosEventos(const QString &nombreDefensa, int tipoDefensa);
    void aplicarCambioDefensaEnTodosEventos(const QString &nombreDefensa, int tipoDefensa, bool activar);
    void actualizarContadorSucesosEnVista(const QString &nombreDefensa, int tipoDefensa);
    void actualizarCheckboxesEnVista(const QString &nombreDefensa, int tipoDefensa, bool activar);
    void recalcularYActualizarRiesgo(QStandardItem *childItem, int eventoId);
    void actualizarRiesgoEnVista(int eventoId, int nuevoRiesgo);
    void actualizarRiesgoEnModelo(QStandardItemModel *model, int eventoId, int nuevoRiesgo);
    void actualizarRobustezEnVista(const QString &nombreDefensa, int tipoDefensa, int nuevaRobustez);
    void actualizarRobustezEnModelo(QStandardItemModel *model, const QString &nombreDefensa, int tipoDefensa, int nuevaRobustez);
    QString obtenerTextoRobustez(int hardiness);
    QString obtenerTextoTipo(DefenseType tipo);
    QString obtenerTextoRiesgo(int riesgo);
    QString obtenerTextoFrecuencia(int frecuencia);
    QString obtenerTextoConsecuencia(int consecuencia);
    QColor obtenerColorRiesgo(int riesgo);
};

#endif // LISTABARRERAS_H
