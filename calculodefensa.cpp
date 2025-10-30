#include "calculodefensa.h"
#include "ui_calculodefensa.h"

#include "dbelementstage.h"
#include "matrizriesgo.h"
#include "defensedefinition.h"

CalculoDefensa::CalculoDefensa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalculoDefensa)
{
    ui->setupUi(this);
}

CalculoDefensa::~CalculoDefensa()
{
    delete ui;
}

void CalculoDefensa::comprobarCalculo(
        const std::unordered_map<std::string, std::vector<CompleteEventDefinition> > &listaEtapas,
        const MatrizRiesgo &matrizRiesgo,
        const QString &nombreDefensa,
        int tipo
) {
    // Comprobación de los que tienen riesgo alto o muy alto

    std::vector<DefenseDefinition> *defensas;

    iniciales.clear();
    listaSucesos.clear();

    // Buscamos los sucesos iniciadores que contengan la defensa buscada.
    for (auto lista : listaEtapas){
        for(CompleteEventDefinition suceso : lista.second) {
            if (tipo == 0) {
                defensas = &(suceso.reductoresFrecuencias);
            } else if (tipo == 1) {
                defensas = &(suceso.barreras);
            } else  {
                defensas = &(suceso.reductoresConsecuencias);
            }
            for(auto &reductor : *defensas){
                if (reductor.nombre == nombreDefensa){
                    listaSucesos.insert(std::make_pair(suceso.nombre.toStdString(), suceso));

                    // Comprueba si tiene RA o RMA para ponerlo en la
                    // primera tabla.
                    if ((suceso.riesgo == 2) || (suceso.riesgo == 3)) {
                        iniciales[suceso.nombre] = suceso.riesgo;
                    }
                }
            }
        }
    }

    std::unordered_map<std::string, std::vector<CompleteEventDefinition> > temporalNada = listaEtapas;
    //std::copy(listaEtapas.begin(), listaEtapas.end(), temporalNada.begin());

    sin.clear();
    bool encontrado = false;
    for(auto &listaEventos : temporalNada){
        for (auto &evento : listaEventos.second) {
            if (tipo == 0) {
                defensas = &(evento.reductoresFrecuencias);
            } else if (tipo == 1) {
                defensas = &(evento.barreras);
            } else  {
                defensas = &(evento.reductoresConsecuencias);
            }
            encontrado = false;
            for(auto &reductor : *defensas){
                if (reductor.nombre == nombreDefensa){
                    encontrado = true;
                    //if (reductor.active) {
                        reductor.active = false;
                    //}
                }
            }
            if (encontrado){
                if (tipo == 0) {
                    evento.calcularReductoresFrecuencia();
                } else if (tipo == 1) {
                    evento.calcularBarreras();
                } else {
                    evento.calcularReductoresConsecuencia();
                }
                evento.calcularRiesgo(matrizRiesgo);
                if ((evento.riesgo == 2) || (evento.riesgo == 3)) {
                    sin[evento.nombre] = evento.riesgo;
                }
            }
        }
    }

    std::unordered_map<std::string, std::vector<CompleteEventDefinition> > temporalTodo = listaEtapas;
    con.clear();
    for(auto &listaEventos : temporalTodo){
        for (auto &evento : listaEventos.second) {
            if (tipo == 0) {
                defensas = &(evento.reductoresFrecuencias);
            } else if (tipo == 1) {
                defensas = &(evento.barreras);
            } else  {
                defensas = &(evento.reductoresConsecuencias);
            }
            encontrado = false;
            for(auto &reductor : *defensas){
                if (reductor.nombre == nombreDefensa){
                    //if (!reductor.active) {
                        //qDebug() << reductor.nombre << " hay que cambiarlo a con";
                        encontrado = true;
                        reductor.active = true;
                    //}
                }
            }
            if (encontrado){
                //qDebug() << "Riesgo inicial: " << evento.riesgo;
                if (tipo == 0) {
                    evento.calcularReductoresFrecuencia();
                } else if (tipo == 1) {
                    evento.calcularBarreras();
                } else {
                    evento.calcularReductoresConsecuencia();
                }
                evento.calcularRiesgo(matrizRiesgo);
                //qDebug() << "Después " << evento.riesgo;
                if ((evento.riesgo == 2) || (evento.riesgo == 3)) {
                    con[evento.nombre] = evento.riesgo;
                }
            }
        }
    }


    rellenarTablas();
}

void CalculoDefensa::rellenarTablas()
{
    // Configuración común para las tres tablas
    auto configurarTabla = [](QTableWidget* tabla) {
        tabla->setColumnCount(3);
        tabla->setSortingEnabled(false);

        tabla->setHorizontalHeaderItem(0, new QTableWidgetItem("Código"));
        tabla->setHorizontalHeaderItem(1, new QTableWidgetItem("Suceso"));
        tabla->setHorizontalHeaderItem(2, new QTableWidgetItem("Riesgo"));

        // Ajustar el ancho de las columnas
        tabla->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        tabla->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tabla->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    };

    // Tabla Actual
    ui->tblActual->setRowCount(static_cast<int>(iniciales.size()));
    configurarTabla(ui->tblActual);

    int srow = 0;
    for(auto l : iniciales) {
        auto it = listaSucesos.find(l.first.toStdString());
        if (it != listaSucesos.end()) {
            QTableWidgetItem* codeItem = new QTableWidgetItem(it->second.codigo);
            QTableWidgetItem* eventItem = new QTableWidgetItem(l.first);
            QTableWidgetItem* riskItem = new QTableWidgetItem(MatrizRiesgo::Riesgos.at(l.second));
            
            // Centrar el contenido
            codeItem->setTextAlignment(Qt::AlignCenter);
            eventItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            riskItem->setTextAlignment(Qt::AlignCenter);
            
            codeItem->setFlags(codeItem->flags() & ~Qt::ItemIsEditable);
            eventItem->setFlags(eventItem->flags() & ~Qt::ItemIsEditable);
            riskItem->setFlags(riskItem->flags() & ~Qt::ItemIsEditable);
            
            ui->tblActual->setItem(srow, 0, codeItem);
            ui->tblActual->setItem(srow, 1, eventItem);
            ui->tblActual->setItem(srow, 2, riskItem);
            srow++;
        }
    }
    ui->tblActual->setSortingEnabled(true);

    // Tabla Sin
    ui->tblSin->setRowCount(static_cast<int>(sin.size()));
    configurarTabla(ui->tblSin);

    srow = 0;
    for(auto l : sin) {
        auto it = listaSucesos.find(l.first.toStdString());
        if (it != listaSucesos.end()) {
            QTableWidgetItem* codeItem = new QTableWidgetItem(it->second.codigo);
            QTableWidgetItem* eventItem = new QTableWidgetItem(l.first);
            QTableWidgetItem* riskItem = new QTableWidgetItem(MatrizRiesgo::Riesgos.at(l.second));
            
            // Centrar el contenido
            codeItem->setTextAlignment(Qt::AlignCenter);
            eventItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            riskItem->setTextAlignment(Qt::AlignCenter);
            
            codeItem->setFlags(codeItem->flags() & ~Qt::ItemIsEditable);
            eventItem->setFlags(eventItem->flags() & ~Qt::ItemIsEditable);
            riskItem->setFlags(riskItem->flags() & ~Qt::ItemIsEditable);
            
            ui->tblSin->setItem(srow, 0, codeItem);
            ui->tblSin->setItem(srow, 1, eventItem);
            ui->tblSin->setItem(srow, 2, riskItem);
            srow++;
        }
    }
    ui->tblSin->setSortingEnabled(true);

    // Tabla Con
    ui->tblCon->setRowCount(static_cast<int>(con.size()));
    configurarTabla(ui->tblCon);

    srow = 0;
    for(auto l : con) {
        auto it = listaSucesos.find(l.first.toStdString());
        if (it != listaSucesos.end()) {
            QTableWidgetItem* codeItem = new QTableWidgetItem(it->second.codigo);
            QTableWidgetItem* eventItem = new QTableWidgetItem(l.first);
            QTableWidgetItem* riskItem = new QTableWidgetItem(MatrizRiesgo::Riesgos.at(l.second));
            
            // Centrar el contenido
            codeItem->setTextAlignment(Qt::AlignCenter);
            eventItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            riskItem->setTextAlignment(Qt::AlignCenter);
            
            codeItem->setFlags(codeItem->flags() & ~Qt::ItemIsEditable);
            eventItem->setFlags(eventItem->flags() & ~Qt::ItemIsEditable);
            riskItem->setFlags(riskItem->flags() & ~Qt::ItemIsEditable);
            
            ui->tblCon->setItem(srow, 0, codeItem);
            ui->tblCon->setItem(srow, 1, eventItem);
            ui->tblCon->setItem(srow, 2, riskItem);
            srow++;
        }
    }
    ui->tblCon->setSortingEnabled(true);
}

void CalculoDefensa::on_btAceptar_clicked()
{
    close();
}
