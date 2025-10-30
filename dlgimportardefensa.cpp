#include "dlgimportardefensa.h"
#include "ui_dlgimportardefensa.h"

#include <algorithm>

#include <QDebug>

#include <matrizriesgo.h>

#include "styles.h"


DlgImportarDefensa::DlgImportarDefensa(QWidget *parent)
{
    DlgImportarDefensa(parent, DefenseType::FrecuencyReducer, nullptr);
}

/**
 * @brief DlgImportarDefensa::DlgImportarDefensa
 * @param parent
 * @param _defense
 * @param currentModel Modelo mostrado en la lista de defensas. Para no mostrar las defensas que ya existen en el modelo
 */
DlgImportarDefensa::DlgImportarDefensa(QWidget *parent, DefenseType _defense, QStandardItemModel *currentModel) :
    QDialog(parent),
    ui(new Ui::DlgImportarDefensa)
{
    ui->setupUi(this);
    //ui->wDureza->setVisible(false);

    ui->listDefenses->setModel(&model);
    defense = _defense;
    actualModel = currentModel;

    listarDefensas();
}

DlgImportarDefensa::~DlgImportarDefensa()
{
    delete ui;
}

void DlgImportarDefensa::listarDefensas()
{
    defensas = Database::DbManager::listDefensesByCodeName(static_cast<int>(defense));

    model.setStringList( QStringList{} );
    //QStringList listaNombresDefensas;
    //int currentIndex = 0;
    actualFilter = actualFilter.toLower();
    for(auto defensa:defensas){
        auto defensasEncontradas = actualModel->findItems(defensa.nombre, Qt::MatchExactly, 0);
        if ((actualFilter.isEmpty() || defensa.nombre.toLower().contains(actualFilter) || defensa.descripcion.toLower().contains(actualFilter)) && defensasEncontradas.isEmpty() ) {
            if(model.insertRow(model.rowCount())) {
                QModelIndex index = model.index(model.rowCount() - 1, 0);
                model.setData(index, "[" + defensa.code + "] " + defensa.nombre);
            }
        }
    }
    if (model.rowCount() > 0) {
        seleccionarDefensa(model.index(0));
    }
}

void DlgImportarDefensa::on_btCancelar_clicked()
{
    selected.code = "";
    selected.nombre = "";
    selected.descripcion = "";
    selected.hardiness = 0;
    close();
}

void DlgImportarDefensa::on_btAceptar_clicked()
{
    accept();
}

void DlgImportarDefensa::seleccionarDefensa(const QModelIndex &index)
{
    QString nombre = model.data(index).toString();
    int posicion = nombre.indexOf(']');
    QString soloNombre = nombre.right(nombre.length() - posicion - 2);
    auto selectedIt = std::find_if(
        defensas.begin(),
        defensas.end(),
        [soloNombre](DefenseDefinition d) { return d.nombre == soloNombre; }
    );
    if (selectedIt != defensas.end()) {
        //selectedIt->orderStageId = model.data(index, Qt::UserRole).toInt();
        ui->etNombre->setText(selectedIt->nombre);
        switch(selectedIt->hardiness){
        case 0:
            ui->etGraficoRobustez->setText("Blanda");
            ui->etGraficoRobustez->setStyleSheet(Styles::WhiteLabel);
            break;
        case 1:
            ui->etGraficoRobustez->setText("Normal");
            ui->etGraficoRobustez->setStyleSheet(Styles::GreenLabel);
            break;
        case 2:
            ui->etGraficoRobustez->setText("Robusta");
            ui->etGraficoRobustez->setStyleSheet(Styles::YellowLabel);
            break;
        case 3:
            ui->etGraficoRobustez->setText("Muy robusta");
            ui->etGraficoRobustez->setStyleSheet(Styles::RedLabel);
            break;
        }
        // ui->etDureza->setText(MatrizRiesgo::Durezas.at(selectedIt->hardiness));
        ui->etCodigo->setText(selectedIt->code);
        ui->etDescripcion->setText(selectedIt->descripcion);
        selected = *selectedIt;
    }
}

void DlgImportarDefensa::on_listDefenses_clicked(const QModelIndex &index)
{
    seleccionarDefensa(index);
}

void DlgImportarDefensa::on_listDefenses_activated(const QModelIndex &)
{
}

void DlgImportarDefensa::on_listDefenses_doubleClicked(const QModelIndex &index)
{
    seleccionarDefensa(index);
    accept();
}

void DlgImportarDefensa::on_edFilter_textChanged(const QString &arg1)
{
    actualFilter = arg1;
    listarDefensas();
}


void DlgImportarDefensa::on_DlgImportarDefensa_rejected()
{
}


void DlgImportarDefensa::on_DlgImportarDefensa_finished(int result)
{
    if (result == 0) {
        on_btCancelar_clicked();
    }
}

