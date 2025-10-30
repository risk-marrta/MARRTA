#include "subetapas.h"
#include "ui_subetapas.h"

#include <QClipboard>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <iostream>
#include <memory>

#include "dbelementsubstage.h"

Subetapas::Subetapas(EtapaDefinition &etapa, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Subetapas)
{
    ui->setupUi(this);

    setWindowTitle("Subetapas de '" + etapa.nombre + "'");

    this->etapa = etapa;

    try{
        auto stages = Database::DbElementSubstage(this->etapa.id);
        stages.list(subetapasDb);
        if (subetapasDb.count() > 0) {
            fillModel();
            showSelectedItem();
        }
    } catch (const char* msg) {
        QMessageBox msgBox;
        msgBox.setText(msg);
        msgBox.exec();
    }
}

Subetapas::~Subetapas()
{
    delete subetapasModel;
    delete ui;
}

void Subetapas::fillModel()
{
    if (subetapasModel != nullptr) {
        delete subetapasModel;
    }
    subetapasModel = new QStandardItemModel(this);
    for(auto &model : subetapasDb) {
        auto item = new QStandardItem(model.nombre);
        item->setData(QVariant::fromValue(model.orden));
        subetapasModel->appendRow(item);
    }
    if (subetapasDb.count() > 0) {
        selectedId = subetapasDb.first().orden;
    } else {
        selectedId = -1;
    }
    ui->listaSubetapas->setModel(subetapasModel);

    connect(ui->listaSubetapas->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        [=] (const QItemSelection &selected) {
            auto selectedIndexes = selected.indexes();
            if (!selectedIndexes.isEmpty()){
                actualIndex = selected.indexes().first();
                selectedId = subetapasModel->itemData(actualIndex).last().toInt();
                showSelectedItem();
            }
        }
    );
}

void Subetapas::showSelectedItem()
{
    if (selectedId >= 0) {
        ui->edNombre->setText(subetapasDb[selectedId].nombre);
        ui->edDescripcion->setText(subetapasDb[selectedId].descripcion);
        ui->cbActiva->setChecked(subetapasDb[selectedId].activo);

        ui->btnSubirOrden->setEnabled(subetapasDb[selectedId].orden != subetapasDb.first().orden);
        ui->btnBajarOrden->setEnabled(subetapasDb[selectedId].orden != subetapasDb.back().orden);
    } else {
        ui->edNombre->setText("");
        ui->edDescripcion->setText("");
        ui->cbActiva->setChecked(false);

        ui->btnSubirOrden->setEnabled(false);
        ui->btnBajarOrden->setEnabled(false);
    }
}

void Subetapas::sortItems()
{
    #ifndef Q_OS_MACOS
        if (subetapasDb.count() > 1) {
            std::sort(subetapasDb.begin(), subetapasDb.end());
        }
    #endif
}

void Subetapas::on_btnCancelar_clicked()
{
    close();
}

void Subetapas::on_btnNueva_clicked()
{
    bool ok;
    QString nombre = QInputDialog::getText(this, "Subetapas", "Indique el nombre de la nueva subetapa", QLineEdit::Normal, "", &ok);
    if (ok && !nombre.isEmpty()) {
        SubetapaDefinition etapa;
        etapa.nombre = nombre;
        etapa.activo = true;
        addOrderableElement<SubetapaDefinition>(subetapasDb, etapa);
        sortItems();
        fillModel();
    }
}

void Subetapas::on_btnEliminar_clicked()
{
    auto [remove, reset] = removeOrderableElement<SubetapaDefinition> (subetapasDb, selectedId);
    if (remove != nullptr) {
        subetapasBorrar << *remove;
    }

    if (reset) {
        fillModel();
        showSelectedItem();
    }
}

void Subetapas::on_btnGuardar_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("¿Quieres guardar los cambios?");
    msgBox.setInformativeText("Si no los guardas perderás los cambios que has realizado");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        QSqlDatabase::database().transaction();

        auto stages = Database::DbElementSubstage(this->etapa.id);
        bool actualizarTodos = false;
        if (subetapasBorrar.count() > 0) {
            actualizarTodos = true;

            try {
                //qDebug() << "Antes de entrar a borrar";
                stages.remove(subetapasBorrar);
            } catch (QString msg) {
                QMessageBox msgBox;
                msgBox.setText(msg);
                msgBox.exec();
                return;
            }
        }

        try {
            stages.update(subetapasDb, actualizarTodos);
        } catch(QString msg) {
            QMessageBox msgBox;
            msgBox.setText(msg);
            msgBox.exec();
            return;
        }

        QSqlDatabase::database().commit();

        QMessageBox msgBox;
        msgBox.setText("Todos los cambios se han guardado.");
        msgBox.exec();

        close();
    }
}

void Subetapas::on_listaSubetapas_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    // Se elimina la posibilidad de copiar y pegar subetapas.
    // La estructura básica hay que definirla a mano

    /*auto item = ui->listaSubetapas->indexAt(pos);
    if (item.isValid()) {
        QMenu menu(this);
        menu.addAction(
            "Copiar",
            [=]{
                //std::cout << subetapasDb[selectedId].orden << std::endl;
                QClipboard *clipboard = QApplication::clipboard();
                QMimeData *data = new QMimeData;
                QByteArray info;
                QDataStream stream(&info, QIODevice::WriteOnly);
                    stream << subetapasDb[selectedId].nombre << subetapasDb[selectedId].descripcion << subetapasDb[selectedId].activo;
                data->setData("stream/marr-subetapa", info);
                clipboard->setMimeData(data);
            }
        );
        menu.exec(ui->listaSubetapas->mapToGlobal(pos));
    } else {
        QMenu menu(this);
        menu.addAction(
            "Pegar",
            [=]{
                const QClipboard *clipboard = QApplication::clipboard();
                auto formats = clipboard->mimeData()->formats();
                for (auto format : formats) {
                    //std::cout << format.toStdString() << std::endl;
                    if (format == "stream/marr-subetapa") {
                        QByteArray info = clipboard->mimeData()->data("stream/marr-subetapa");
                        SubetapaDefinition etapa;
                        QDataStream stream(&info, QIODevice::ReadOnly);
                        stream >> etapa.nombre >> etapa.descripcion >> etapa.activo;
                        addOrderableElement<SubetapaDefinition>(subetapasDb, etapa);
                        sortItems();
                        fillModel();
                    }
                }
            }
        );
        menu.exec(ui->listaSubetapas->mapToGlobal(pos));
    }*/
}

void Subetapas::on_btnSubirOrden_clicked()
{
    changeOrder<SubetapaDefinition>(subetapasDb, selectedId, false);
    sortItems();
    fillModel();
}

void Subetapas::on_btnBajarOrden_clicked()
{
    changeOrder<SubetapaDefinition>(subetapasDb, selectedId, true);
    sortItems();
    fillModel();
}
