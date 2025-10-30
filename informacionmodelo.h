#ifndef INFORMACIONMODELO_H
#define INFORMACIONMODELO_H

#include <QDialog>

#include "dbmanager.h"

namespace Ui {
class InformacionModelo;
}

class InformacionModelo : public QDialog
{
    Q_OBJECT

public:
    explicit InformacionModelo(Database::ConfiguracionModelo _configuracionModelo, QWidget *parent = nullptr);
    ~InformacionModelo();

    bool modelChanged() { return modificado; }

    Database::ConfiguracionModelo configuracion() { return configuracionModelo; }

private slots:
    void on_edNombre_textChanged(const QString &arg1);

    void on_edTecnica_textChanged(const QString &arg1);

    void on_edAnalistas_textChanged();

    void on_edInstitucion_textChanged(const QString &arg1);

    void on_edDescripcion_textChanged();

    void on_edObservaciones_textChanged();

    void on_btCancelar_clicked();

    void on_btGuardar_clicked();

private:
    Ui::InformacionModelo *ui;

    Database::ConfiguracionModelo configuracionModelo;
    bool modificado;

private:
    void initModel();
};

#endif // INFORMACIONMODELO_H
