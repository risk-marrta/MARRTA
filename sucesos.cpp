#include "sucesos.h"
#include "ui_sucesos.h"

#include <QWizard>
#include <QInputDialog>
#include <QRadioButton>
#include <QGroupBox>

#include <QMenu>
#include <QMimeData>
#include <QClipboard>
#include <QDataStream>
#include <QMap>

#include <algorithm>

#include <QDebug>

#include "eventhomepage.h"
#include "defensas.h"
#include "defensecalc.h"
#include "dbelementstage.h"
#include "dbevent.h"

#include "styles.h"
#include "waitingspinnerwidget.h"

#include "mainwindow.h"

sucesos::sucesos(QWidget *parent, bool v) :
    QDialog(parent),
    ui(new Ui::sucesos),
    verbose(v)
{
    ui->setupUi(this);
    ui->eventEditor->setVisible(false);
    eventosModel = nullptr;
    preseleccionar = nullptr;
    selectedStagePair = std::make_pair(ElementType::Scene, std::make_pair(-1,-1));
    setupModel();

    eventQuestions.valueForValue1 = -1;
    eventQuestions.valueForValue2 = 0;
    eventQuestions.valueForValue3 = 1;
    eventQuestions.valueForValue4 = 2;

    Question question1(0, "¿Afecta a un solo paciente o a varios?");

    Answer answer1_1(1, "Un paciente", 0);
    Answer answer2_1(2, "Varios pacientes", 1);
    question1.answers.push_back(answer1_1);
    question1.answers.push_back(answer2_1);
    eventQuestions.questions.push_back(question1);
    eventQuestions.userSelections[0] = 0;

    Question question2(1, "¿Tiene consecuencias graves, recuperables o sin relevancia clínica?");
    Answer answer1_2(3, "Sin relevancia", 0);
    Answer answer2_2(4, "Recuperables", 1);
    Answer answer2_3(5, "Graves", 2);
    question2.answers.push_back(answer1_2);
    question2.answers.push_back(answer2_2);
    question2.answers.push_back(answer2_3);

    eventQuestions.questions.push_back(question2);
    eventQuestions.userSelections[1] = 0;
}

sucesos::~sucesos()
{
    delete model;
    delete eventosModel;
    delete ui;
}

void sucesos::establecerSucesoActivo(const CompleteEventDefinition &suceso)
{
    int rows = model->rowCount();
    ElementIndexType element;
    for (int i = 0; i < rows; i++) {
        auto item = model->item(i);
        element = item->data().value<ElementIndexType>();
        //qDebug() << element.second.first;

        if (suceso.etapa == element.second.first) {
            if (suceso.subetapa == 0) {
                ui->stages->selectionModel()->setCurrentIndex(
                    model->indexFromItem(item),
                    QItemSelectionModel::SelectCurrent
                );

                preseleccionar = const_cast<CompleteEventDefinition *>(&suceso);
                return;
            } else {
                int numSubetapas = item->rowCount();
                for (int j = 0; j < numSubetapas; j++) {
                    auto subitem = item->child(j);
                    if (suceso.subetapa == subitem->data().value<ElementIndexType>().second.second) {
                        ui->stages->selectionModel()->setCurrentIndex(
                            model->indexFromItem(subitem),
                            QItemSelectionModel::SelectCurrent
                        );

                        preseleccionar = const_cast<CompleteEventDefinition*>(&suceso);
                        return;
                    }
                }
            }
        }
    }
}

void sucesos::fillModel()
{
    if (eventosModel != nullptr) {
        delete eventosModel;
    }
    eventosModel = new QStandardItemModel(this);
    for(auto &model : eventosDb) {
        auto item = new QStandardItem("[" + model.codigo + "] " + model.nombre);
        item->setData(QVariant::fromValue(model), Qt::UserRole);
        
        // ** apm ** Formateo del tooltip para que el texto se divida en líneas de aproximadamente 100 caracteres,
        // rompiendo por palabras completas. Esto evita que el tooltip se expanda horizontalmente más allá
        // de lo deseado y mejora su legibilidad, especialmente en configuraciones multi-monitor.
        QString tooltipText = model.descripcion;
        QString formattedTooltip;
        int lineLength = 0;
        QStringList words = tooltipText.split(' ');
        
        for (const QString& word : words) {
            if (lineLength + word.length() > 100) {
                formattedTooltip += "\n";
                lineLength = 0;
            }
            formattedTooltip += word + " ";
            lineLength += word.length() + 1;
        }
        
        item->setToolTip(formattedTooltip.trimmed());
        eventosModel->appendRow(item);
    }
    actualIndex = QModelIndex();
    ui->sucesosList->setModel(eventosModel);

    connect(ui->sucesosList->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        [=] (const QItemSelection &selected) {
            actualIndex = selected.indexes().first();
            showSelectedItem();
        }
    );

    // Cuando se ha buscado un evento, después de seleccionar la etapa/subetapa
    // establecerá preseleccionar al evento que hay que seleccionar
    if (preseleccionar != nullptr) {
        qDebug() << preseleccionar->nombre << eventosModel->rowCount();

        int numSucesos = eventosModel->rowCount();
        QStandardItem *item;
        EventDefinition eventItem;
        for (int i = 0; i < numSucesos; i++) {
            item = eventosModel->item(i);
            eventItem = qvariant_cast<EventDefinition>(item->data(Qt::UserRole));
            qDebug() << eventItem.nombre << "##" << preseleccionar->nombre;
            if (eventItem.nombre == preseleccionar->nombre && eventItem.codigo == preseleccionar->codigo) {
                ui->sucesosList->selectionModel()->setCurrentIndex(item->index(), QItemSelectionModel::SelectCurrent);
                ui->sucesosList->setFocus();
                return;
            }
        }

        preseleccionar = nullptr;
    }

    showSelectedItem();
}

void sucesos::getEvent(ElementType elementType, std::pair<int, int> id)
{
    Database::DbEvent dbEvent;
    try {
        if (elementType == ElementType::Scene) {
            dbEvent.list(eventosDb, id.first, 0);
        } else {
            dbEvent.list(eventosDb, id.first, id.second);
        }

        selectedStagePair = std::make_pair(elementType, id);
        modificado = false;
        fillModel();
    } catch(QString msg) {
        selectedStagePair = std::make_pair(elementType, std::make_pair(-1,-1));

        QMessageBox msgBox;
        msgBox.setText(msg);
        msgBox.exec();
        return;
    }
}

void sucesos::showSelectedItem()
{
    if (actualIndex.isValid()) {
        selectedEvent = qvariant_cast<EventDefinition>(eventosModel->data(actualIndex, Qt::UserRole));
        ui->edName->setText(selectedEvent.nombre);
        ui->edName->home(false);
        ui->edCode->setText(selectedEvent.codigo);
        ui->edDescription->setPlainText(selectedEvent.descripcion);
        ui->cbHuman->setChecked(selectedEvent.errorHumano);
        ui->cbFrequency->setCurrentIndex(selectedEvent.frecuencia);
        ui->cbConsequence->setCurrentIndex(selectedEvent.consecuencia);

        ui->btnNuevoEvento->setEnabled(true);
        ui->btnEliminarEvento->setEnabled(true);

        ui->eventEditor->setVisible(true);
        calcMinimumRisk();
    } else if (selectedStagePair.second.first < 0) {
        selectedEvent.nombre = "";
        ui->eventEditor->setVisible(false);
        ui->edName->setText("");
        ui->edDescription->setPlainText("");
        ui->cbHuman->setChecked(false);
        ui->cbFrequency->setCurrentIndex(0);
        ui->cbConsequence->setCurrentIndex(0);

        ui->btnNuevoEvento->setEnabled(false);
        ui->btnEliminarEvento->setEnabled(false);
    } else {
        ui->eventEditor->setVisible(false);

        ui->btnNuevoEvento->setEnabled(true);
        ui->btnEliminarEvento->setEnabled(false);
    }
}

void sucesos::onSelectionChanged(const QModelIndex &index, const QModelIndex &previous)
{
    if (modificado == true) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmación", "Has modificado los datos de este suceso. Si cambias sin guardar se perderán los cambios. ¿Quieres continuar?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            modificado = false;
            //auto sm = ui->stages->selectionModel();
            ui->stages->selectionModel()->setCurrentIndex(previous, QItemSelectionModel::SelectCurrent);
            return;
        }
    }
    QStandardItem *item = model->itemFromIndex(index);

    ElementIndexType elementId = item->data().value<ElementIndexType>();
    //qDebug() << item->text() << " clicked " << ((elementId.first == ElementType::Scene) ? "Escena" : "Subescena") << ((elementId.first == ElementType::Scene) ? elementId.second.first : elementId.second.second);

    getEvent(elementId.first, elementId.second);
}

void sucesos::setupModel()
{
    model = new QStandardItemModel();

    auto stages = Database::DbElementStage();
    stages.list(etapasDb);
    if (etapasDb.count() > 0) {
        for (auto stage : etapasDb) {
            // Add [codigo] prefix to stage name
            QStandardItem *item = new QStandardItem("[" + stage.codigo + "] " + stage.nombre);
            ElementIndexType elementId(ElementType::Scene, std::make_pair(stage.id, 0));
            item->setData(QVariant::fromValue(elementId));

            auto substages = Database::DbElementSubstage(stage.id);
            substages.list(subetapasDb);

            // Check if stage has any events
            QList<EventDefinition> stageEvents;
            Database::DbEvent dbEvent;
            dbEvent.list(stageEvents, stage.id, 0);
            bool hasStageEvents = !stageEvents.isEmpty();

            // Check if any substage has events
            bool hasSubstageEvents = false;
            for (auto sub : subetapasDb) {
                QList<EventDefinition> substageEvents;
                // Reuse dbEvent instance for substage queries
                dbEvent.list(substageEvents, stage.id, sub.id);
                
                //if(verbose) qDebug() << sub.nombre;
                QStandardItem *subitem = new QStandardItem(sub.nombre);
                ElementIndexType subelementId(ElementType::Subscene, std::make_pair(stage.id, sub.id));
                subitem->setData(QVariant::fromValue(subelementId));

                // If substage has no events, set it to gray
                if (substageEvents.isEmpty()) {
                    subitem->setForeground(QBrush(QColor(128, 128, 128))); // Gray color
                } else {
                    hasSubstageEvents = true;
                }

                item->appendRow(subitem);
            }

            // If neither stage nor its substages have events, set stage to gray
            if (!hasStageEvents && !hasSubstageEvents) {
                item->setForeground(QBrush(QColor(128, 128, 128))); // Gray color
            }

            model->appendRow(item);
        }
    }

    ui->stages->setModel(model);
    auto selection_model = ui->stages->selectionModel();
    connect(selection_model, &QItemSelectionModel::currentChanged, this, &sucesos::onSelectionChanged, Qt::QueuedConnection);

    // Initialize the selected event to a not valid scene
    selectedStagePair = std::make_pair(ElementType::Scene, std::make_pair(-1, -1));

    // Initialize the combos
    ui->cbFrequency->addItem("Muy baja");
    ui->cbFrequency->addItem("Baja");
    ui->cbFrequency->addItem("Media");
    ui->cbFrequency->addItem("Alta");

    ui->cbConsequence->addItem("Baja");
    ui->cbConsequence->addItem("Media");
    ui->cbConsequence->addItem("Alta");
    ui->cbConsequence->addItem("Muy alta");
}

void sucesos::calcMinimumRisk()
{
    if (actualIndex.isValid()) {
        //auto selected = qvariant_cast<EventDefinition>(eventosModel->data(actualIndex, Qt::UserRole));

        Database::DbDefense defensas;
        //qDebug() << "Calculando reductores para " << eventosDb[selectedId].id;
        int frecuencyReducer = DefenseCalc::getFrecuencyReducer(
                    defensas.groupByHardiness(selectedEvent.id, DefenseType::FrecuencyReducer, false)
        );
        int barrier = DefenseCalc::getBarriers(
                    defensas.groupByHardiness(selectedEvent.id, DefenseType::Barrier, false)
        );
        int consecuencyReducer = DefenseCalc::getConsequencyReducer(
                    defensas.groupByHardiness(selectedEvent.id, DefenseType::ConsequenceReducer, false)
        );

        int riesgoMinimo = matrizRiesgo.riesgo(
                    static_cast <std::size_t>(DefenseCalc::calcProbabilityLevel(selectedEvent, barrier)),
                    static_cast <std::size_t>(DefenseCalc::calcFrecuencyLevel(selectedEvent, frecuencyReducer)),
                    static_cast <std::size_t>(DefenseCalc::calcConsequencyLevel(selectedEvent, consecuencyReducer))
        );
        // El riesgo máximo será el cálculo cuando no hay ninguna defensa marcada
        int riesgoMaximo = matrizRiesgo.riesgo(3, selectedEvent.frecuencia, selectedEvent.consecuencia);

        switch (riesgoMinimo) {
            case 0:
                ui->etRiesgoMinimo->setText("Riesgo Bajo (RB)");
                ui->etRiesgoMinimo->setStyleSheet(Styles::WhiteLabel);
                break;
            case 1:
                ui->etRiesgoMinimo->setText("Riesgo Medio (RM)");
                ui->etRiesgoMinimo->setStyleSheet(Styles::GreenLabel);
                break;
            case 2:
                ui->etRiesgoMinimo->setText("Riesgo Alto (RA)");
                ui->etRiesgoMinimo->setStyleSheet(Styles::YellowLabel);
                break;
            case 3:
                ui->etRiesgoMinimo->setText("Riesgo Muy Alto (RMA)");
                ui->etRiesgoMinimo->setStyleSheet(Styles::RedLabel);
                break;
        }
        switch (riesgoMaximo) {
            case 0:
                ui->etRiesgoMaximo->setText("Riesgo Bajo (RB)");
                ui->etRiesgoMaximo->setStyleSheet(Styles::WhiteLabel);
                break;
            case 1:
                ui->etRiesgoMaximo->setText("Riesgo Medio (RM)");
                ui->etRiesgoMaximo->setStyleSheet(Styles::GreenLabel);
                break;
            case 2:
                ui->etRiesgoMaximo->setText("Riesgo Alto (RA)");
                ui->etRiesgoMaximo->setStyleSheet(Styles::YellowLabel);
                break;
            case 3:
                ui->etRiesgoMaximo->setText("Riesgo Muy Alto (RMA)");
                ui->etRiesgoMaximo->setStyleSheet(Styles::RedLabel);
                break;
        }
    }
}

int sucesos::calcResult(EventQuestion &eventQuestion) {
    //qDebug() << "Respuesta leídas: " << eventQuestion.userSelections[0] << "-" << eventQuestion.userSelections[1] << Qt::endl;
    // 1A uno - 2C sin efectos
    if ((eventQuestion.userSelections[0] == 0) && (eventQuestion.userSelections[1] == 0)) {
        return 0;
    } else if ( // 1A uno - 2B recuperable O 1B varios - 2C sin efectos
        ((eventQuestion.userSelections[0] == 0) && (eventQuestion.userSelections[1] == 1)) ||
        ((eventQuestion.userSelections[0] == 1) && (eventQuestion.userSelections[1] == 0))) {
        return 1;
    } else if ( // 1A uno - 2A grave  O  1B varios - 2B recuperable
       ((eventQuestion.userSelections[0] == 0) && (eventQuestion.userSelections[1] == 2)) ||
       ((eventQuestion.userSelections[0] == 1) && (eventQuestion.userSelections[1] == 1))) {
        return 2;
    } else {
        return 3;
    }
}

QWizardPage *sucesos::createQuestionPage(const Question &question)
{
    QWizardPage *page = new QWizardPage;
    page->setTitle(question.text);

    QVBoxLayout *layout = new QVBoxLayout;
    // Add margins to prevent text from being too close to the edges
    layout->setContentsMargins(10, 10, 20, 10); // left, top, right, bottom
    
    for (auto answer = ::std::begin(question.answers), first = answer; answer != std::end(question.answers); ++answer) {
        QRadioButton *button = new QRadioButton(answer->text);
        button->setProperty("_answer_value", answer->value);

        if (answer == first) {
            button->setChecked(true);
        }
        connect(button,
                &QAbstractButton::toggled,
                [=](bool checked) {
                    if (checked) {
                        //qDebug() << "Respuesta anterior a " << question.id << ": " << eventQuestions.userSelections[question.id];
                        eventQuestions.userSelections[question.id] = button->property("_answer_value").toInt();
                        //qDebug() << "Respuesta actual a " << question.id << ": " << eventQuestions.userSelections[question.id];
                    }
                }
        );
        layout->addWidget(button);
    }
    page->setLayout(layout);

    return page;
}

void sucesos::createConsequencesPages(QWizard &wizard)
{

    QWizardPage *consequenceResume = new QWizardPage;
    QLabel *consequenceResumeText = new QLabel("En las próximas páginas se le realizarán algunas preguntas para obtener las consecuencias del evento.");
    QVBoxLayout *consequenceLayout = new QVBoxLayout;
    // Add margins to prevent text from being too close to the edges
    consequenceLayout->setContentsMargins(10, 10, 20, 10); // left, top, right, bottom
    consequenceLayout->addWidget(consequenceResumeText);
    consequenceResume->setLayout(consequenceLayout);
    wizard.addPage(consequenceResume);


    for (Question question : eventQuestions.questions) {
        wizard.addPage(createQuestionPage(question));
    }
    wizard.setStartId(0);
}

void sucesos::createFrequencyPages(QWizard &wizard)
{
    QWizardPage *frecuencyPage = new QWizardPage;
    frecuencyPage->setTitle("Indique el valor más apropiado para la frecuencia del evento");

    QVBoxLayout *frecuencyLayout = new QVBoxLayout;
    // Add margins to prevent text from being too close to the edges
    frecuencyLayout->setContentsMargins(10, 10, 20, 10); // left, top, right, bottom

    std::vector<std::pair<int, QString>> textosFrecuencias{
        std::make_pair(0, "Menos de 1 suceso cada 100 años (F < 0, 01)"),
        std::make_pair(1, "Entre 1 suceso por año y 1 suceso cada 100 años (0,01 < F < 1)"),
        std::make_pair(2, "Entre 1 y 50 sucesos por año (1 < F < 50)"),
        std::make_pair(3, "Más de 50 sucesos en un año (F > 50)")
    };
    eventQuestions.frecuency = 0;

    for (auto texto : textosFrecuencias) {
        QRadioButton *frecuency = new QRadioButton(texto.second);
        frecuency->setProperty("_frecuency_value", texto.first);

        if (texto.first == 0) {
            frecuency->setChecked(true);
        }
        connect(frecuency,
                &QAbstractButton::toggled,
                [=](bool checked) {
                    if (checked) {
                        eventQuestions.frecuency = frecuency->property("_frecuency_value").toInt();
                    }
        });
        frecuencyLayout->addWidget(frecuency);
    }
    frecuencyPage->setLayout(frecuencyLayout);
    wizard.addPage(frecuencyPage);
}

void sucesos::sortItems()
{
    #ifndef Q_OS_MACOS
        std::sort(eventosDb.begin(), eventosDb.end());
    #endif
}

void sucesos::on_btnNuevoEvento_clicked()
{
    QWizard wizard;
    int etapaId = selectedStagePair.second.first;
    QString newEventCode = Database::DbElementStage::getNextAvailableCode(etapaId);

    wizard.addPage(new EventHomePage(proximoCodigo(etapaId)));

    // Creación de páginas para las preguntas
    createFrequencyPages(wizard);
    createConsequencesPages(wizard);
    wizard.setWindowTitle("Creación de suceso iniciador");

    #ifndef Q_OS_MAC
      wizard.setWizardStyle(QWizard::ModernStyle);
      wizard.setPixmap(QWizard::WatermarkPixmap, QPixmap(":/comun/resources/comun/matriz-de-riesgos.png"));
    #else
      wizard.setWizardStyle(QWizard::MacStyle);
      wizard.setPixmap(QWizard::BackgroundPixmap, QPixmap(":/comun/resources/comun/matriz-de-riesgos.png"));
    #endif

    wizard.setButtonText(QWizard::CancelButton, "Cancelar");
    wizard.setButtonText(QWizard::FinishButton, "Crear evento");
    wizard.setButtonText(QWizard::NextButton, "Siguiente");
    wizard.setButtonText(QWizard::BackButton, "Anterior");
    
    // Set wizard size: wider and shorter
    wizard.resize(700, 450);

    if (wizard.exec() == QDialog::Accepted) {
        EventDefinition event;
        event.etapa.id = etapaId;
        event.subetapa.id = (selectedStagePair.first == ElementType::Subscene) ?
                    selectedStagePair.second.second : -1;
        event.nombre = wizard.field("name").toString();
        event.codigo = wizard.field("code").toString();
        event.descripcion = wizard.field("description").toString();
        event.frecuencia = eventQuestions.frecuency;
        event.consecuencia = calcResult(eventQuestions);
        event.errorHumano = wizard.field("human").toBool();

        Database::DbEvent dbEvent;
        try {
            dbEvent.create(event);
        } catch (QString msg) {
            QMessageBox msgBox;
            msgBox.setText(msg);
            msgBox.exec();
            return;
        }

        getEvent(selectedStagePair.first, selectedStagePair.second);
        
        // Update the tree to remove gray color if this was the first event for this stage/substage
        updateTreeItemColor(selectedStagePair);
    }
}

void sucesos::on_btSave_clicked()
{
    if (modificado == true) {
        QMessageBox msgBox;
        msgBox.setText("¿Quieres guardar los cambios?");
        msgBox.setInformativeText("Si no los guardas perderás los cambios que has realizado");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Save) {
            qDebug() << selectedEvent.nombre;
            Database::DbEvent::updateElement(selectedEvent);
            modificado = false;

            accept();
        }
    }
}

void sucesos::on_edName_textEdited(const QString &arg1)
{
    if (actualIndex.isValid()) {
        selectedEvent.nombre = arg1.simplified();
        modificado = true;
    }
}

void sucesos::on_edCode_textEdited(const QString &arg1)
{
    if (actualIndex.isValid()) {
        selectedEvent.codigo = arg1.simplified();
        modificado = true;
    }
}

void sucesos::on_edDescription_textChanged()
{
    if (actualIndex.isValid()) {
        if (selectedEvent.descripcion != ui->edDescription->toPlainText()) {
            selectedEvent.descripcion = ui->edDescription->toPlainText().simplified();
            modificado = true;
        }
    }
}

/*
 * Cuando se establece como fallo humano, la frecuencia no puede ser muy baja.
 */
void sucesos::on_cbHuman_stateChanged(int arg1)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->cbFrequency->model());
    Q_ASSERT(model != nullptr);
    QStandardItem *item = model->item(0);
    bool disabled = false;

    if (actualIndex.isValid()) {

        // If checked
        if (arg1 == 2) {
            if (selectedEvent.frecuencia == 0) {
                selectedEvent.frecuencia = 1;
                ui->cbFrequency->setCurrentIndex(1);
                calcMinimumRisk();
            }

            disabled = true;
        }

        item->setFlags(disabled ? item->flags() & ~Qt::ItemIsEnabled
                                : item->flags() | Qt::ItemIsEnabled
        );

        if (selectedEvent.errorHumano != ui->cbHuman->isChecked()) {
            selectedEvent.errorHumano = ui->cbHuman->isChecked(); //qDebug() << "Error humano";
            modificado = true;
        }
    }
}

void sucesos::on_cbFrequency_activated(int index)
{
    if (actualIndex.isValid()) {
        if (selectedEvent.frecuencia != index) {
            selectedEvent.frecuencia = index; qDebug() << "Frecuencia" << index;
            modificado = true;
            calcMinimumRisk();
        }
    }
}

void sucesos::on_cbConsequence_activated(int index)
{
    if (actualIndex.isValid()) {
        if (selectedEvent.consecuencia != index) {
            selectedEvent.consecuencia = index; qDebug() << "Consecuencia" << index;
            modificado = true;
            calcMinimumRisk();
        }
    }
}

void sucesos::on_btFrecuencyReducers_clicked()
{
    if (actualIndex.isValid()) {
        Defensas defensa(this, selectedEvent.id, DefenseType::FrecuencyReducer);
        defensa.cambiarTitulo("Reductores de frecuencia", ui->edName->text());
        defensa.exec();
    }
}

void sucesos::on_btBarriers_clicked()
{
    if (actualIndex.isValid()) {
        Defensas defensa(this, selectedEvent.id, DefenseType::Barrier);
        defensa.cambiarTitulo("Barreras", ui->edName->text());
        defensa.exec();
    }
}

void sucesos::on_btConsequenceReducers_clicked()
{
    if (actualIndex.isValid()) {
        Defensas defensa(this, selectedEvent.id, DefenseType::ConsequenceReducer);
        defensa.cambiarTitulo("Reductores de consecuencia", ui->edName->text());
        defensa.exec();
    }
}


void sucesos::on_btCancel_clicked()
{
    if (modificado == true) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmación", "Has modificado los datos de este suceso. ¿Quieres guardar los cambios antes de cerrar?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            on_btSave_clicked();
        }
    }

    accept();
}

void sucesos::on_btnEliminarEvento_clicked()
{
    if (actualIndex.isValid()) {
        if (QMessageBox::warning(this, "MARRTA", "Eliminación de iniciador\n"
            "¿Quieres continuar con la eliminación de este iniciador?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes) == QMessageBox::Yes
        ) {
            if(Database::DbEvent::remove(selectedEvent)) {
                auto previousSelected = selectedStagePair;
                setupModel();
                getEvent(previousSelected.first, previousSelected.second);
            } else {
                QMessageBox msgBox;
                msgBox.setText("No se ha podido borrar el evento");
                msgBox.exec();
            }
        }
    }
}

void sucesos::defensasAStream(QDataStream &stream, Database::DbDefense &defensas, DefenseType tipo)
{
    if (actualIndex.isValid()) {
        auto selected = qvariant_cast<EventDefinition>(eventosModel->data(actualIndex, Qt::UserRole));
        QList<DefenseDefinition> defensasDb;
        defensas.list(defensasDb, selected.id, tipo);
        quint32 numDefensas = static_cast<quint32>(defensasDb.count());
        stream << numDefensas;

        if (defensasDb.count() > 0) {
        for(auto defensa : defensasDb) {
            stream << defensa;
        }
    }

    }
}

QList<DefenseDefinition> sucesos::defensasDesdeStream(QDataStream &stream, DefenseType tipo)
{
    QList<DefenseDefinition> defensas;
    quint32 numDefensas;
    stream >> numDefensas;

    for (quint32 i = 0; i < numDefensas; i++) {
        DefenseDefinition defensa;
        stream >> defensa;
        //qDebug() << defensa.nombre << defensa.descripcion << defensa.orden;
        defensa.type = tipo;
        defensas.push_back(defensa);
    }

    return defensas;
}

QString sucesos::proximoCodigo(int etapaABuscar)
{
    auto etapaActual = std::find_if(
        etapasDb.begin(),
        etapasDb.end(),
        [etapaABuscar] (auto etapa) {
            return etapa.id == etapaABuscar;
        }
    );
    if (etapaActual == etapasDb.end()) {
        throw "Etapa no encontrada";
    }

    // Busco el último código usado en esta etapa
    Database::DbEvent dbEvent;
    QStringList listaSucesosBd = dbEvent.listCodesFromEventsWithStage(etapaActual->id);

    int maxNumCodigo = 0;
    for (auto suceso : listaSucesosBd) {
        QStringList codigosEtapa = suceso.split("-");
        if (codigosEtapa.count() == 2) {
            int numCodigo = codigosEtapa[1].toInt();
            if (numCodigo > maxNumCodigo) {
                maxNumCodigo = numCodigo;
            }
        }
    }

    return etapaActual->codigo + "-" + QStringLiteral("%1").arg(maxNumCodigo + 1, 2, 10, QLatin1Char('0'));
}

void sucesos::on_sucesosList_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->sucesosList->indexAt(pos);
    if (item.isValid()) {
        QMenu menu(this);
        menu.addAction(
            "Copiar",
            [=]{
                if (actualIndex.isValid()) {
                    auto selected = qvariant_cast<EventDefinition>(eventosModel->data(actualIndex, Qt::UserRole));
                    QClipboard *clipboard = QApplication::clipboard();
                    QMimeData *data = new QMimeData;
                    QByteArray info;
                    QDataStream stream(&info, QIODevice::WriteOnly);

                    auto defenses = Database::DbDefense();
                    QList<DefenseDefinition> defensasDb;

                    defensasAStream(stream, defenses, DefenseType::FrecuencyReducer);
                    defensasAStream(stream, defenses, DefenseType::Barrier);
                    defensasAStream(stream, defenses, DefenseType::ConsequenceReducer);

                    // Información del evento
                    stream << selected;

                    data->setData("stream/marr-evento", info);
                    clipboard->setMimeData(data);
                }
            }
        );
        menu.exec(ui->sucesosList->mapToGlobal(pos));
    } else {
        QMenu menu(this);
        menu.addAction(
            "Pegar",
            [=]{
                const QClipboard *clipboard = QApplication::clipboard();
                auto formats = clipboard->mimeData()->formats();
                for (auto format : formats) {
                    if (format == "stream/marr-evento") {
                        QByteArray info = clipboard->mimeData()->data("stream/marr-evento");
                        EventDefinition evento;
                        QDataStream stream(&info, QIODevice::ReadOnly);

                        WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(this, true, true);
                        spinner->start();
                        QCoreApplication::processEvents();

                        stream >> evento;
                        evento.etapa.id = selectedStagePair.second.first;
                        evento.subetapa.id = selectedStagePair.second.second;

                        // Busqueda de la etapa por id
                        if (etapasDb.count() <= 0) {
                            return; // Esto no debería pasar nunca
                        }

                        bool ok;
                        QString nuevoCodigo = QInputDialog::getText(
                            this,
                            "Creación de suceso iniciador",
                            "Use el código del suceso iniciador que está pegando o indique un nuevo código",
                            QLineEdit::Normal,
                            proximoCodigo(selectedStagePair.second.first),
                            &ok
                        );
                        if (ok && !nuevoCodigo.isEmpty()) {
                            evento.codigo = nuevoCodigo;
                        } else {
                            spinner->stop();
                            return;
                        }
                        QString nuevoTitulo = QInputDialog::getText(this, "Creación de suceso iniciador", "Use el nombre del suceso iniciador que está pegando o indique uno nuevo", QLineEdit::Normal, evento.nombre, &ok);
                        if (ok && !nuevoTitulo.isEmpty()) {
                            evento.nombre = nuevoTitulo;
                        } else {
                            spinner->stop();
                            return;
                        }

                        // Creación del nuevo suceso iniciador
                        // idQ contendrá, si se ha creado bien, el id del nuevo suceso iniciador
                        QVariant idQ;
                        try {
                            Database::DbEvent dbEvent;
                            idQ = dbEvent.create(evento);

                            if (idQ.isValid()){
                                int id = idQ.toInt();
                                Database::DbDefense dbDefensas;

                                QList<DefenseDefinition> reductoresFrecuencia = defensasDesdeStream(stream, DefenseType::FrecuencyReducer);
                                QList<DefenseDefinition> barreras = defensasDesdeStream(stream, DefenseType::Barrier);
                                QList<DefenseDefinition> reductoresConsecuencia = defensasDesdeStream(stream, DefenseType::ConsequenceReducer);

                                dbDefensas.createForEvent(reductoresFrecuencia, id);
                                dbDefensas.createForEvent(barreras, id);
                                dbDefensas.createForEvent(reductoresConsecuencia, id);

                                getEvent(selectedStagePair.first, selectedStagePair.second);

                                spinner->stop();
                            } else {
                                spinner->stop();

                                QMessageBox msgBox;
                                msgBox.setText("Error al crear las defensas para el evento iniciador.");
                                msgBox.exec();
                            }
                        } catch(...) {
                            spinner->stop();

                            QMessageBox msgBox;
                            msgBox.setText("Error al crear el nuevo suceso iniciador.");
                            msgBox.exec();
                        }
                    }
                }
            }
        );
        menu.exec(ui->sucesosList->mapToGlobal(pos));
    }
}

void sucesos::on_cbFrequency_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}

void sucesos::updateTreeItemColor(const std::pair<ElementType, std::pair<int, int>>& elementPair)
{
    // Find the corresponding item in the tree model
    int stageId = elementPair.second.first;
    int substageId = elementPair.second.second;
    
    // Search for the stage item in the model
    for (int i = 0; i < model->rowCount(); i++) {
        QStandardItem* stageItem = model->item(i);
        ElementIndexType stageElementId = stageItem->data().value<ElementIndexType>();
        
        if (stageElementId.second.first == stageId) {
            // Found the stage
            // Query all events for this stage once to avoid multiple database queries
            QList<EventDefinition> allStageEvents;
            Database::DbEvent dbEvent;
            dbEvent.listAllForStage(allStageEvents, stageId);
            
            // Build a map of substage IDs to event counts for quick lookup
            QMap<int, int> substageEventCounts;
            int stageOnlyEventCount = 0;
            
            for (const auto& event : allStageEvents) {
                int eventSubstageId = event.subetapa.id;
                if (eventSubstageId == 0) {
                    stageOnlyEventCount++;
                } else {
                    substageEventCounts[eventSubstageId]++;
                }
            }
            
            if (elementPair.first == ElementType::Scene) {
                // Update stage color - check if it now has events
                if (stageOnlyEventCount > 0) {
                    // Remove gray color (set to default/black)
                    stageItem->setData(QVariant(), Qt::ForegroundRole);
                }
            } else {
                // Update substage color
                for (int j = 0; j < stageItem->rowCount(); j++) {
                    QStandardItem* substageItem = stageItem->child(j);
                    ElementIndexType substageElementId = substageItem->data().value<ElementIndexType>();
                    
                    if (substageElementId.second.second == substageId) {
                        // Check if substage now has events using cached results
                        if (substageEventCounts.value(substageId, 0) > 0) {
                            // Remove gray color (set to default/black)
                            substageItem->setData(QVariant(), Qt::ForegroundRole);
                            
                            // Check if stage or any substage has events
                            bool hasAnyEvents = (stageOnlyEventCount > 0) || !substageEventCounts.isEmpty();
                            
                            if (hasAnyEvents) {
                                stageItem->setData(QVariant(), Qt::ForegroundRole);
                            }
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
}
