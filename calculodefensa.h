#ifndef CALCULODEFENSA_H
#define CALCULODEFENSA_H

#include <QDialog>
#include <unordered_map>
#include "completeeventdefinition.h"

namespace Ui {
class CalculoDefensa;
}

class CalculoDefensa : public QDialog
{
    Q_OBJECT

public:
    explicit CalculoDefensa(QWidget *parent = nullptr);
    ~CalculoDefensa();

    /**
     * @brief comprobarCalculo obtiene los eventos que se encuentran actualmente con riesgo RA o RMA y los que cambiarían si la defensa está presente o no.
     * @param listaEtapas                   Lista de los eventos actuales
     * @param matrizRiesgo  MatrizRiesgo    Matriz para hacer el cálculo del riesgo
     * @param nombreDefensa QString         Nombre de la defensa a cambiar
     * @param tipo          int             0 - RF | 1 - B | 2 - RC
     */
    void comprobarCalculo(const std::unordered_map<std::string, std::vector<CompleteEventDefinition> > &listaEtapas, const MatrizRiesgo &matrizRiesgo, const QString &nombreDefensa, int tipo);

private slots:
    void on_btAceptar_clicked();

private:
    Ui::CalculoDefensa *ui;

    std::map<QString, int> iniciales;
    std::map<QString, int> sin;
    std::map<QString, int> con;
    std::unordered_map<std::string, CompleteEventDefinition> listaSucesos;

private:
    void rellenarTablas();
};

#endif // CALCULODEFENSA_H
