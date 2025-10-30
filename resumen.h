#ifndef RESUMEN_H
#define RESUMEN_H

#include <QDialog>

#include "dbelementstage.h"
#include "completeeventdefinition.h"

// Forward declarations
QT_BEGIN_NAMESPACE
class QChartView;
QT_END_NAMESPACE

namespace Ui {
class Resumen;
}

class Resumen : public QDialog
{
    Q_OBJECT

public:
    explicit Resumen(std::unordered_map<std::string, std::vector<CompleteEventDefinition> > *_events, QWidget *parent = nullptr);
    ~Resumen();

private slots:
    void on_btRiskSourceText_clicked();
    void on_btExportFrequencyChart_clicked();
    void on_btExportConsequenceChart_clicked();
    void on_btExportRiskChart_clicked();

private:
    Ui::Resumen *ui;

    std::unordered_map<std::string, std::vector<CompleteEventDefinition> > *events;
    std::vector<Summary> summary;
    MatrizRiesgo matrizRiesgo;
    QList<EtapaDefinition> etapasDb;

    // Chart views for export
    QChartView *frequencyChartView;
    QChartView *consequenceChartView;
    QChartView *riskChartView;

private:
    void getSummary();
    void drawSummaryChart();
    void drawFrequencyChart();
    void drawConsequenceChart();
    QString drawHigherRiskInitiator();
    QString drawHighRiskInitiator();
    QString drawMediumRiskInitiator();

    QString drawRiskInitiatorTableHeader(QString title);
    
    void exportChartToPNG(QChartView *chartView, const QString &defaultFileName);
};

#endif // RESUMEN_H
