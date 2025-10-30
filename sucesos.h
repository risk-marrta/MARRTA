#ifndef SUCESOS_H
#define SUCESOS_H

#include <unordered_map>

#include <QDialog>
#include <QStandardItemModel>
#include <QWizard>
#include <QSharedPointer>

#include "etapas.h"
#include "subetapas.h"
#include "dbmanager.h"
#include "dbelementstage.h"
#include "dbelementsubstage.h"
#include "dbevent.h"
#include "dbdefense.h"

#include "orderablewidget.h"
#include "matrizriesgo.h"

namespace Ui {
class sucesos;
}

enum class ElementType {
    Scene,
    Subscene
};

struct Answer {
    int id;
    QString text;
    int value;

    Answer(int _id, QString _text, int _value):
        id{_id},
        text{_text},
        value{_value} {}
};

struct Question {
   int id;
   QString text;
   std::vector<Answer> answers;

   Question(int _id, QString _text):
       id{_id},
       text{_text} {}
};

struct EventQuestion {
   std::vector<Question> questions;
   std::unordered_map<int, int> userSelections;
   int frecuency;
   int probability;
   QString name;
   QString code;
   QString description;
   int valueForValue1;
   int valueForValue2;
   int valueForValue3;
   int valueForValue4;
};

typedef QPair<ElementType, std::pair<int,int>> ElementIndexType;    // para identificar si es etapa o subetapa y su índice
Q_DECLARE_METATYPE(ElementIndexType)


class sucesos : public QDialog
{
    Q_OBJECT

public:
    explicit sucesos(QWidget *parent = nullptr, bool v = false);
    ~sucesos();

    void establecerSucesoActivo(const CompleteEventDefinition &suceso);

private slots:
    void on_btnNuevoEvento_clicked();

    void on_cbHuman_stateChanged(int arg1);

    void on_btFrecuencyReducers_clicked();

    void on_btBarriers_clicked();

    void on_btConsequenceReducers_clicked();

    void on_edDescription_textChanged();

    void on_btCancel_clicked();

    void on_btSave_clicked();

    void onSelectionChanged(const QModelIndex &, const QModelIndex &);

    void on_edName_textEdited(const QString &arg1);

    void on_edCode_textEdited(const QString &arg1);

    void on_cbFrequency_activated(int index);

    void on_cbConsequence_activated(int index);

    void on_btnEliminarEvento_clicked();

    void on_sucesosList_customContextMenuRequested(const QPoint &pos);

    void on_cbFrequency_currentIndexChanged(int index);

private:
    Ui::sucesos *ui;
    QList<EtapaDefinition> etapasDb;
    QList<SubetapaDefinition> subetapasDb;
    QStandardItemModel* model;
    QStandardItemModel *eventosModel;
    //int selectedId;
    QModelIndex actualIndex;
    MatrizRiesgo matrizRiesgo;
    std::pair<ElementType, std::pair<int, int>> selectedStagePair;
    EventDefinition selectedEvent;

    QList<EventDefinition> eventosDb;
    QList<EventDefinition> eventosBorrar;
    bool modificado = false;
    EventQuestion eventQuestions;
    bool verbose;
    CompleteEventDefinition *preseleccionar;

private:
    void setupModel();

    int calcResult(EventQuestion &eventQuestion);
    /*void calcDefaultRisk();*/
    void calcMinimumRisk();
    void createConsequencesPages(QWizard &wizard);
    void createFrequencyPages(QWizard &wizard);
    QWizardPage *createQuestionPage(const Question &question);

    void defensasAStream(QDataStream &, Database::DbDefense &, DefenseType);
    QList<DefenseDefinition> defensasDesdeStream(QDataStream &, DefenseType);

    void getEvent(ElementType, std::pair<int, int>);

    void fillModel();
    QString proximoCodigo(int etapaABuscar);
    void showSelectedItem();
    void sortItems();
    void updateTreeItemColor(const std::pair<ElementType, std::pair<int, int>>& elementPair);
};

#endif // SUCESOS_H
