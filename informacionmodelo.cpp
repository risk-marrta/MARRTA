#include "informacionmodelo.h"
#include "ui_informacionmodelo.h"

#include <QDebug>

InformacionModelo::InformacionModelo(Database::ConfiguracionModelo _configuracionModelo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformacionModelo)
{
    ui->setupUi(this);
    configuracionModelo = _configuracionModelo;
    qDebug() << "Modelo " << _configuracionModelo.nombre << _configuracionModelo.tecnica;
    initModel();
}

InformacionModelo::~InformacionModelo()
{
    delete ui;
}

void InformacionModelo::initModel()
{
    ui->edNombre->setText(configuracionModelo.nombre);
    ui->edTecnica->setText(configuracionModelo.tecnica);
    ui->edAnalistas->setPlainText(configuracionModelo.analistas);
    ui->edInstitucion->setText(configuracionModelo.institucion);
    ui->edDescripcion->setPlainText(configuracionModelo.descripcion);
    ui->edObservaciones->setPlainText(configuracionModelo.observaciones);

    modificado = false;
}

void InformacionModelo::on_edNombre_textChanged(const QString &arg1)
{
    configuracionModelo.nombre = arg1;
    modificado = true;
}

void InformacionModelo::on_edTecnica_textChanged(const QString &arg1)
{
    configuracionModelo.tecnica = arg1;
    modificado = true;
}

void InformacionModelo::on_edAnalistas_textChanged()
{
    configuracionModelo.analistas = ui->edAnalistas->toPlainText();
    modificado = true;
}

void InformacionModelo::on_edInstitucion_textChanged(const QString &arg1)
{
    configuracionModelo.institucion = arg1;
    modificado = true;
}

void InformacionModelo::on_edDescripcion_textChanged()
{
    configuracionModelo.descripcion = ui->edDescripcion->toPlainText();
    modificado = true;
}

void InformacionModelo::on_edObservaciones_textChanged()
{
    configuracionModelo.observaciones = ui->edObservaciones->toPlainText();
    modificado = true;
}

void InformacionModelo::on_btCancelar_clicked()
{
    modificado = false;
    close();
}

void InformacionModelo::on_btGuardar_clicked()
{
    close();
}
