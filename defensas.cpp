#include "defensas.h"
#include "ui_defensas.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QMimeData>
#include <QClipboard>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <algorithm>

#include "dbdefense.h"
#include "dbelementstage.h"
#include "defensecalc.h"
#include "matrizriesgo.h"

#include "dlgimportardefensa.h"

#include "styles.h"

Defensas::Defensas(QWidget *parent, int _eventId, DefenseType _defense) :
    QDialog(parent),
    ui(new Ui::Defensas)
{
    defensasModel = nullptr;
    ui->setupUi(this);
    listOrderStages();

    if (_defense == DefenseType::FrecuencyReducer) {
        ui->cbHardiness->addItem("Formación (blanda)", QVariant::fromValue(0));
        ui->cbHardiness->addItem("Protocolos y carga de trabajo (normal)", QVariant::fromValue(1));
        ui->cbHardiness->addItem("Señales (robusta)", QVariant::fromValue(2));
        ui->cbHardiness->addItem("Mejoras tecnológicas (muy robusta)", QVariant::fromValue(3));
    } else if (_defense == DefenseType::ConsequenceReducer) {
        ui->cbHardiness->addItem("Controles de QA, auditorías internas (muy blanda)", QVariant::fromValue(-1));
        ui->cbHardiness->addItem("Protocolos y procedimientos (blanda)", QVariant::fromValue(0));
        ui->cbHardiness->addItem("Revisión médico/TR (normal)", QVariant::fromValue(1));
        ui->cbHardiness->addItem("Señales/alarmas (robusta)", QVariant::fromValue(2));
        ui->cbHardiness->addItem("Enclavamientos (muy robusta)", QVariant::fromValue(3));
    } else {
        ui->cbHardiness->addItem("Tarea de una persona (blanda)", QVariant::fromValue(0));
        ui->cbHardiness->addItem("Procedimiento por varias personas / verificación independiente (normal)", QVariant::fromValue(1));
        ui->cbHardiness->addItem("Alarma (robusta)", QVariant::fromValue(2));
        ui->cbHardiness->addItem("Enclavamiento (muy robusta)", QVariant::fromValue(3));
    };


    if (_eventId != -1) {
        establecerEvento(_eventId, _defense);
    }
}

Defensas::~Defensas()
{
    delete ui;
}

void Defensas::cambiarTitulo(QString tipoDefensa, QString suceso)
{
    setWindowTitle(QString("Gestión de defensas") + " - " + tipoDefensa);
    ui->etSuceso->setText(suceso);
}

void Defensas::cargarDefensas()
{
    try{
        auto defenses = Database::DbDefense();
        defenses.list(defensasDb, eventId, defense);
        fillModel();
        showSelectedItem();
    } catch (QString msg) {
        QMessageBox msgBox;
        msgBox.setText(msg);
        msgBox.exec();
    }
}

void Defensas::establecerEvento(int _eventId, DefenseType _defense)
{
    eventId = _eventId;
    defense = _defense;
    cargarDefensas();
}

void Defensas::fillModel()
{
    if (defensasModel != nullptr) {
        delete defensasModel;
    }
    defensasModel = new QStandardItemModel(this);
    for(auto &model : defensasDb) {
        auto item = new QStandardItem(model.nombre);
        item->setData(QVariant::fromValue(model.orden));
        defensasModel->appendRow(item);
    }
    if (defensasDb.count() > 0) {
        selectedId = defensasDb.first().orden;
    } else {
        selectedId = -1;
    }
    ui->defenseList->setModel(defensasModel);

    connect(ui->defenseList->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        [=] (const QItemSelection &selected) {
            auto selectedIndexes = selected.indexes();
            if (!selectedIndexes.isEmpty()){
                actualIndex = selectedIndexes.first();
                selectedId = defensasModel->itemData(actualIndex).last().toInt();
                showSelectedItem();
            }
        }
    );
}

/**
 * @brief Defensas::listOrderStages establece los valores del combo
 * con la lista de etapas que se pueden usar para ordenar.
 */
void Defensas::listOrderStages()
{
    ui->cbEtapasOrden->clear();
    etapaOrdenInicial = 0;
    etapaOrdenValorInicial = 0;

    try {
        etapasOrden = Database::DbElementStage::listNamesCodes();

        for(auto etapa : etapasOrden) {
            ui->cbEtapasOrden->addItem(QString::fromStdString(etapa.second.name), QVariant::fromValue(etapa.first));
        }
    }catch (QString msg) {
        qDebug() << "Error cargando la lista de etapas por orden";
    }
}

void Defensas::showSelectedItem()
{
    if ((selectedId >= 0) && (selectedId < defensasDb.size())) {
        // Valores usados como temporales
        etapaOrdenInicial = defensasDb[selectedId].orderStageId;
        etapaOrdenValorInicial = defensasDb[selectedId].bdOrden;
        codigoInicial = defensasDb[selectedId].code;

        // UI
        ui->edName->setText(defensasDb[selectedId].nombre);
        ui->edName->home(false);
        ui->edCode->setText(defensasDb[selectedId].code);
        // En los reductores de consecuencias se puede encontrar la fortaleza con valor -1 (muy blanda)
        // Por lo que hay que sumar 1 al valor que hay en la base de datos
        if (defense == DefenseType::ConsequenceReducer) {
            ui->cbHardiness->setCurrentIndex(defensasDb[selectedId].hardiness + 1);
        } else {
            ui->cbHardiness->setCurrentIndex(defensasDb[selectedId].hardiness);
        }
        ui->edDescripcion->setPlainText(defensasDb[selectedId].descripcion);
        if (etapaOrdenInicial != 0) {
            int index = ui->cbEtapasOrden->findData(defensasDb[selectedId].orderStageId);
            ui->cbEtapasOrden->setCurrentIndex(index);
        } else {
            ui->cbEtapasOrden->setCurrentIndex(0);
        }
    } else {
        ui->edName->setText("");
        ui->cbHardiness->setCurrentIndex(0);
    }

    ui->btnSave->setEnabled(false);
}

void Defensas::sortItems()
{
    #ifndef Q_OS_MACOS
        std::sort(defensasDb.begin(), defensasDb.end());
    #endif
}

void Defensas::on_btnNewDefense_clicked()
{
    bool ok;
    QString nombre = QInputDialog::getText(this, "Defensa", "Indique el nombre de la nueva defensa", QLineEdit::Normal, "", &ok);
    if (ok && !nombre.isEmpty()) {
        // Primero busco si se ha creado una defensa con el mismo nombre antes.
        DefenseDefinition defensaAnterior = Database::DbDefense::searchByName(nombre.simplified());

        DefenseDefinition defensa;

        if (nombre.simplified() == defensaAnterior.nombre) {
            QMessageBox::critical(this, "MARRTA", "Este nombre ya está siendo utilizado.");
            return;
        }
        defensa.nombre = nombre.simplified();
        defensa.orden = 1;
        defensa.orderStageId = 1;
        defensa.hardiness = 0;


        defensa.event = eventId;
        defensa.type = defense;

        // Número según la tabla inicial
        //qDebug() << etapasOrden.at(defensa.orderStageId);
        qDebug() << etapasOrden.at(defensa.orderStageId).code;
        QString defenseCode = etapasOrden.at(defensa.orderStageId).code.c_str();
        int numericalDefenseCode = Database::DbElementStage::getNextAvailableDefenseCode(primeraParteCodigoDefensa() + "-" + defenseCode);
            //etapasOrden.at(defensa.orderStageId).max + 1;

        //numericalDefenseCode = comprobarCodigosActuales(defenseCode, numericalDefenseCode);

        defensa.code = primeraParteCodigoDefensa()
            + "-"
            + defenseCode
            + "-"
            + QStringLiteral("%1").arg(numericalDefenseCode, 3, 10, QLatin1Char('0'));

        Database::DbDefense defense;
        defense.create(defensa);

        cargarDefensas();
        //selectedId = defensasDb.size() - 1;

        // Hay que buscar el id que se le ha dado al recien creado
        for (unsigned i = 0; i < defensasDb.size(); i++) {
            if (defensasDb[i].code == defensa.code) {
                selectedId = i;
            }
        }

        showSelectedItem();
    }
}

int Defensas::comprobarCodigosActuales(QString defenseCode, int numericalDefenseCode)
{
    QRegularExpression re(".{3}-"  + defenseCode + "-(\\d\\d)");

    for (auto d : defensasDb) {
        QRegularExpressionMatch match = re.match(d.code);
        if (match.hasMatch()) {
            int capturedCode = match.captured(1).toInt();
            if (capturedCode >= numericalDefenseCode) {
                numericalDefenseCode = capturedCode + 1;
            }
        }
    }

    return numericalDefenseCode;
}

QString Defensas::primeraParteCodigoDefensa()
{
    if (defense == DefenseType::Barrier) {
        return "BA";
    } else if (defense == DefenseType::FrecuencyReducer) {
        return "RF";
    }// else if (defense == DefenseType::ConsequenceReducer) {
        return "RC";
    //}
}

void Defensas::on_edName_textChanged(const QString &arg1)
{
    if (selectedId >= 0 && defensasDb.count() > selectedId) {
        defensasDb[selectedId].setName(arg1.simplified());
        ui->btnSave->setEnabled(true);
    }
}

void Defensas::on_edCode_textChanged(const QString &arg1)
{
    if (selectedId >= 0 && defensasDb.count() > selectedId) {
        defensasDb[selectedId].code = arg1.simplified();
        ui->btnSave->setEnabled(true);
    }
}

/**
 * @brief Defensas::on_cbHardiness_currentIndexChanged comprueba si hay algún
 * elemento seleccionado y obtiene la información del data del elemento.
 * @param index
 */
void Defensas::on_cbHardiness_currentIndexChanged(Q_DECL_UNUSED int index)
{
    if (selectedId >= 0 && defensasDb.count() > selectedId) {
        //qDebug() << ui->cbHardiness->currentData().toInt() << "\n";

        defensasDb[selectedId].setHardiness(ui->cbHardiness->currentData().toInt());
        ui->btnSave->setEnabled(true);
    }

    switch(ui->cbHardiness->currentData().toInt()){
        case -1:
            ui->etGraficoRobustez->setText("Muy blanda");
            ui->etGraficoRobustez->setStyleSheet(Styles::WhiteLabel);
            break;
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
}

void Defensas::on_btnRemoveDefense_clicked()
{
    if ((selectedId >= 0) && (defensasDb.count() > selectedId)) {
        Database::DbDefense::remove(defensasDb[selectedId]);
        cargarDefensas();
    }
}

void Defensas::on_btnCancel_clicked()
{
    close();
}

void Defensas::on_btnSave_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("¿Quieres guardar los cambios?");
    msgBox.setInformativeText("Si no los guardas puedes perder los cambios que has realizado");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        DefenseDefinition defensaActualizar = defensasDb[selectedId];
        defensaActualizar.nombre = ui->edName->text();
        defensaActualizar.code = ui->edCode->text();
        defensaActualizar.hardiness = ui->cbHardiness->currentData().toInt();
        defensaActualizar.descripcion = ui->edDescripcion->toPlainText();
        defensaActualizar.orderStageId = ui->cbEtapasOrden->currentIndex();

        qDebug() << defensaActualizar.id << ": " << ui->cbHardiness->currentData().toInt() << "\n";

        auto error = Database::DbDefense::updateElement(defensaActualizar);
        if (error.isEmpty()) {
            cargarDefensas();
        } else {
            QMessageBox::warning(this, "MARRTA", "Error al guardar:\n" + error);
        }
    }
}

void Defensas::on_defenseList_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->defenseList->indexAt(pos);
    if (item.isValid()) {
        QMenu menu(this);
        menu.addAction(
            "Copiar",
            [=]{
                QClipboard *clipboard = QApplication::clipboard();
                QMimeData *data = new QMimeData;
                QByteArray info;
                QDataStream stream(&info, QIODevice::WriteOnly);
                stream
                        << defensasDb[selectedId].nombre
                        << defensasDb[selectedId].descripcion
                        << defensasDb[selectedId].hardiness
                        << defensasDb[selectedId].orderStageId
                        << static_cast<int>(defensasDb[selectedId].type);
                data->setData("stream/marr-defensa", info);
                clipboard->setMimeData(data);
            }
        );
        
        menu.exec(ui->defenseList->mapToGlobal(pos));
    } else {
        QMenu menu(this);
        menu.addAction(
            "Pegar",
            [=]{
                const QClipboard *clipboard = QApplication::clipboard();
                auto formats = clipboard->mimeData()->formats();
                for (auto format : formats) {
                    if (format == "stream/marr-defensa") {
                        QByteArray info = clipboard->mimeData()->data("stream/marr-defensa");
                        DefenseDefinition defensa;
                        QDataStream stream(&info, QIODevice::ReadOnly);
                        int tipoPegado;
                        stream >> defensa.nombre >> defensa.descripcion >> defensa.hardiness >> defensa.orderStageId >> tipoPegado;
                        if (tipoPegado == static_cast<int>(defense)) {
                            addOrderableElement<DefenseDefinition>(defensasDb, defensa);

                            sortItems();
                            fillModel();
                        } else {
                            QMessageBox msgBox;
                            msgBox.setText("No puedes pegar defensas de distinto tipo");
                            msgBox.exec();
                        }
                    }
                }
            }
        );
        menu.exec(ui->defenseList->mapToGlobal(pos));
    }
}

void Defensas::on_edDescripcion_textChanged()
{
    if (selectedId >= 0 && defensasDb.count() > selectedId) {
        defensasDb[selectedId].descripcion = ui->edDescripcion->toPlainText();
        ui->btnSave->setEnabled(true);
        //qDebug() << ui->edDescripcion->toPlainText();
    }
}

void Defensas::on_cbEtapasOrden_currentIndexChanged(int index)
{
    //qDebug() << selectedId;
    //qDebug() << index;
    if (selectedId >= 0) {
        if (index >= 0) {
            //index = 0;
            //ui->cbEtapasOrden->setCurrentIndex(0);
            //}
            int ordenElegido = ui->cbEtapasOrden->itemData(index).toInt();

            // Comprobación para saber si se ha elegido el orden inicial
            if (ordenElegido != etapaOrdenInicial) {
                int nuevoOrden = 1;
                auto orderElement = etapasOrden.find(ordenElegido);
                if (orderElement != etapasOrden.end()) {
                    nuevoOrden = orderElement->second.max + 1;
                }
                defensasDb[selectedId].orderStageId = ordenElegido;
                defensasDb[selectedId].bdOrden = nuevoOrden;

                // Número según la tabla inicial
                QString defenseCode = etapasOrden.at(ordenElegido).code.c_str();
                int numericalDefenseCode = Database::DbElementStage::getNextAvailableDefenseCode(primeraParteCodigoDefensa() + "-" + defenseCode);

                /*int numericalDefenseCode = etapasOrden.at(ordenElegido).max + 1;
                QString defenseCode = etapasOrden.at(ordenElegido).code.c_str();
                numericalDefenseCode = comprobarCodigosActuales(defenseCode, numericalDefenseCode);*/

                ui->edCode->setText (primeraParteCodigoDefensa()
                        + "-"
                        + defenseCode
                        + "-"
                        + QStringLiteral("%1").arg(numericalDefenseCode, 3, 10, QLatin1Char('0'))
                );
                etapaOrdenInicial = nuevoOrden;
            } else {
                defensasDb[selectedId].orderStageId = ordenElegido;
                defensasDb[selectedId].bdOrden = etapaOrdenValorInicial;
                ui->edCode->setText(codigoInicial);
            }
        }
    }
}

/*void Defensas::on_edOrden_valueChanged(int arg1)
{
    if (selectedId >= 0) {
        if (arg1 != defensasDb[selectedId].bdOrden) {
            defensasDb[selectedId].bdOrden = arg1;
        }
    }
}*/

/**
 * @brief Defensas::on_toolButton_clicked Importa defensas
 */
void Defensas::on_toolButton_clicked()
{
    DlgImportarDefensa dlgImportarDefensa(this, defense, defensasModel);
    dlgImportarDefensa.exec();

    // Comprueba si en el diálogo se ha seleccionado alguna defensa.
    // En ese caso lo añade a la lista.
    auto defensa = dlgImportarDefensa.getSelected();
    if (!defensa.nombre.isEmpty()){
        defensa.event = this->eventId;
        addOrderableElement<DefenseDefinition>(defensasDb, defensa);


        Database::DbDefense defense;
        defense.create(defensa);

        cargarDefensas();

        sortItems();
        fillModel();

        //ui->btnSave->setEnabled(true);
    }
}

void Defensas::on_btRecargarCodigo_clicked()
{
    auto order = defensasDb[selectedId].orderStageId;
    auto etapa = std::find_if(etapasOrden.begin(), etapasOrden.end(), [order](auto ee) { return ee.second.order == order; });

    ui->edCode->setText (primeraParteCodigoDefensa()
            + "-"
            + etapa->second.code.c_str()
            + "-"
            + QStringLiteral("%1").arg(etapa->second.max + 1, 2, 10, QLatin1Char('0'))
    );
}

void Defensas::recalcularRiesgoEvento(int eventoId)
{
    // Primero obtener datos base del evento
    QSqlQuery qryEvento;
    qryEvento.prepare("SELECT frecuencia, consecuencia, error_humano FROM suceso_iniciador WHERE id = :eventoId");
    qryEvento.bindValue(":eventoId", eventoId);
    
    if (!qryEvento.exec() || !qryEvento.next()) {
        qDebug() << "Error al obtener evento:" << qryEvento.lastError().text();
        return;
    }
    
    int frecuenciaBase = qryEvento.value(0).toInt();
    int consecuenciaBase = qryEvento.value(1).toInt();
    bool errorHumano = qryEvento.value(2).toBool();
    
    // Ahora obtener todas las defensas del evento agrupadas por tipo y robustez
    QSqlQuery qry;
    qry.prepare("SELECT tipo, robustez FROM defensa WHERE suceso_iniciador_id = :eventoId AND activo = 1 ORDER BY tipo");
    qry.bindValue(":eventoId", eventoId);
    
    if (!qry.exec()) {
        qDebug() << "Error al obtener defensas:" << qry.lastError().text();
        return;
    }
    
    // Contar defensas por tipo y robustez
    int reductoresFrecuencia[4] = {0, 0, 0, 0}; // hardiness 0-3
    int barreras[4] = {0, 0, 0, 0};
    int reductoresConsecuencia[4] = {0, 0, 0, 0};
    
    while (qry.next()) {
        int tipo = qry.value(0).toInt();
        int robustez = qry.value(1).toInt();
        
        if (robustez >= 0 && robustez <= 3) {
            if (tipo == 0) { // Reductor de frecuencia
                reductoresFrecuencia[robustez]++;
            } else if (tipo == 1) { // Barrera
                barreras[robustez]++;
            } else if (tipo == 2) { // Reductor de consecuencia
                reductoresConsecuencia[robustez]++;
            }
        }
    }
    
    // Preparar datos para DefenseCalc
    std::vector<std::pair<int, int>> infoReductoresFrecuencia;
    for (int i = 0; i < 4; i++) {
        infoReductoresFrecuencia.push_back(std::make_pair(i, reductoresFrecuencia[i]));
    }
    
    std::vector<std::pair<int, int>> infoBarreras;
    for (int i = 0; i < 4; i++) {
        infoBarreras.push_back(std::make_pair(i, barreras[i]));
    }
    
    std::vector<std::pair<int, int>> infoReductoresConsecuencia;
    for (int i = 0; i < 4; i++) {
        infoReductoresConsecuencia.push_back(std::make_pair(i, reductoresConsecuencia[i]));
    }
    
    // Calcular usando DefenseCalc
    int reductorFrecuencia = DefenseCalc::getFrecuencyReducer(infoReductoresFrecuencia);
    int frecuenciaCalc = DefenseCalc::calcFrecuencyLevel(frecuenciaBase, errorHumano, reductorFrecuencia);
    
    int reductorBarreras = DefenseCalc::getBarriers(infoBarreras);
    int probabilidadCalc = DefenseCalc::calcProbabilityLevel(reductorBarreras);
    
    int reductorConsecuencia = DefenseCalc::getConsequencyReducer(infoReductoresConsecuencia);
    int consecuenciaCalc = DefenseCalc::calcConsequencyLevel(consecuenciaBase, reductorConsecuencia);
    
    // Calcular riesgo con MatrizRiesgo
    MatrizRiesgo matriz;
    int riesgoCalc = matriz.riesgo(
        static_cast<std::size_t>(probabilidadCalc),
        static_cast<std::size_t>(frecuenciaCalc),
        static_cast<std::size_t>(consecuenciaCalc)
    );
    
    // Actualizar en la base de datos
    QSqlQuery qryUpdate;
    qryUpdate.prepare("UPDATE suceso_iniciador SET riesgo = :riesgo WHERE id = :eventoId");
    qryUpdate.bindValue(":riesgo", riesgoCalc);
    qryUpdate.bindValue(":eventoId", eventoId);
    
    if (!qryUpdate.exec()) {
        qDebug() << "Error al actualizar riesgo:" << qryUpdate.lastError().text();
    }
}

