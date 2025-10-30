#include "resumen.h"
#include "ui_resumen.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts>

#include <QGridLayout>
#include <QColor>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>

#include "matrizriesgo.h"

#include "styles.h"

Resumen::Resumen(std::unordered_map<std::string, std::vector<CompleteEventDefinition> > *_events, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Resumen),
    frequencyChartView(nullptr),
    consequenceChartView(nullptr),
    riskChartView(nullptr)
{
    ui->setupUi(this);
    ui->tblSummary->setColumnCount(6);

   this->events = _events;
    getSummary();

    drawSummaryChart();
    drawFrequencyChart();
    drawConsequenceChart();

    QString riskMarkdownText = drawHigherRiskInitiator();
    riskMarkdownText += drawHighRiskInitiator();
    riskMarkdownText += drawMediumRiskInitiator();

    ui->tIniciadoresMayorRiesgo->setText(riskMarkdownText);
}

Resumen::~Resumen()
{
    delete ui;
}


void Resumen::getSummary()
{
    auto stages = Database::DbElementStage();

    for(std::size_t i = 0; i < Database::DbElementStage::HeaderTitles.size(); i++) {
        ui->tblSummary->setHorizontalHeaderItem(static_cast<int>(i), new QTableWidgetItem(Database::DbElementStage::HeaderTitles[i]));
    }


    summary = stages.summary(events);
    stages.list(etapasDb);

    ui->tblSummary->setRowCount(static_cast<int>(summary.size()) + 1);

    int rma = 0, ra = 0, rm = 0, rb = 0, srow = 0;
    for(auto s : summary) {
        //qDebug() << sDefinition.name << sDefinition.rma << sDefinition.ra << sDefinition.rm << sDefinition.rb;
        QTableWidgetItem nameItem(s.name);
        ui->tblSummary->setItem(srow, 0, new QTableWidgetItem(s.name));
        ui->tblSummary->setItem(srow, 1, new QTableWidgetItem(QString::number(s.rma)));
        ui->tblSummary->setItem(srow, 2, new QTableWidgetItem(QString::number(s.ra)));
        ui->tblSummary->setItem(srow, 3, new QTableWidgetItem(QString::number(s.rm)));
        ui->tblSummary->setItem(srow, 4, new QTableWidgetItem(QString::number(s.rb)));
        ui->tblSummary->setItem(srow, 5, new QTableWidgetItem(QString::number(s.rma + s.ra + s.rm + s.rb)));
        for(int column=1; column<6; column++){
            ui->tblSummary->item(srow,column)->setTextAlignment(Qt::AlignCenter);
        }

        rma += s.rma;
        ra += s.ra;
        rm += s.rm;
        rb += s.rb;
        srow++;
    }

    ui->tblSummary->setItem(srow, 0, new QTableWidgetItem("Total"));
    ui->tblSummary->item(srow, 0)->setBackground(Qt::white);
    ui->tblSummary->item(srow, 0)->setForeground(Qt::black);
    ui->tblSummary->setItem(srow, 1, new QTableWidgetItem(QString::number(rma)));
    ui->tblSummary->item(srow, 1)->setBackground(Qt::white);
    ui->tblSummary->item(srow, 1)->setForeground(Qt::black);
    ui->tblSummary->setItem(srow, 2, new QTableWidgetItem(QString::number(ra)));
    ui->tblSummary->item(srow, 2)->setBackground(Qt::white);
    ui->tblSummary->item(srow, 2)->setForeground(Qt::black);
    ui->tblSummary->setItem(srow, 3, new QTableWidgetItem(QString::number(rm)));
    ui->tblSummary->item(srow, 3)->setBackground(Qt::white);
    ui->tblSummary->item(srow, 3)->setForeground(Qt::black);
    ui->tblSummary->setItem(srow, 4, new QTableWidgetItem(QString::number(rb)));
    ui->tblSummary->item(srow, 4)->setBackground(Qt::white);
    ui->tblSummary->item(srow, 4)->setForeground(Qt::black);

    ui->tblSummary->setColumnWidth(0,230);
    for(int column=0; column<5; column++){
        ui->tblSummary->item(srow,column)->setTextAlignment(Qt::AlignCenter);
        ui->tblSummary->setColumnWidth(column+1,130);
    }

    //ui->tblSummary->resizeColumnsToContents();



}

QString Resumen::drawHigherRiskInitiator()
{
    QString tabla;

    QSqlQuery *initiators = new QSqlQuery();
    initiators->prepare(R"""(SELECT codigo,nombre,frecuencia,consecuencia,riesgo FROM suceso_iniciador WHERE riesgo=:risk AND completado=1)""");

    initiators->bindValue(":risk", 3);
    if (initiators->exec()) {
        int frecuencia, consecuencia;
        bool cabeceraMostrada = false;

        while(initiators->next()) {
            if (!cabeceraMostrada) {
                tabla.append(this->drawRiskInitiatorTableHeader("alto (RA)"));
                cabeceraMostrada = true;
            }
            frecuencia = initiators->value(2).toInt();
            consecuencia = initiators->value(3).toInt();

            tabla.append(
                "|" + initiators->value(0).toString() +
                "|" + initiators->value(1).toString() +
                "|" + MatrizRiesgo::Frecuencias.at(frecuencia) +
                "|" + MatrizRiesgo::Consecuencias.at(consecuencia) +
                "|" + QChar(matrizRiesgo.riesgoReferencia(frecuencia, consecuencia)) +
                "|\n"
             );
        }
    }
    tabla.append("\n\n\n\n");

    delete initiators;

    return tabla;
}

QString Resumen::drawHighRiskInitiator()
{
    QString tabla;

    QSqlQuery *initiators = new QSqlQuery();
    initiators->prepare(R"""(SELECT codigo,nombre,frecuencia,consecuencia,riesgo FROM suceso_iniciador WHERE riesgo=:risk AND completado=1)""");

    initiators->bindValue(":risk", 2);
    if (initiators->exec()) {
        int frecuencia, consecuencia;
        bool cabeceraMostrada = false;

        while(initiators->next()) {
            if (!cabeceraMostrada) {
                tabla.append(this->drawRiskInitiatorTableHeader("alto (RA)"));
                cabeceraMostrada = true;
            }
            frecuencia = initiators->value(2).toInt();
            consecuencia = initiators->value(3).toInt();
            qDebug() << matrizRiesgo.riesgoReferencia(frecuencia, consecuencia);

            tabla.append(
                "|" + initiators->value(0).toString() +
                "|" + initiators->value(1).toString() +
                "|" + MatrizRiesgo::Frecuencias.at(frecuencia) +
                "|" + MatrizRiesgo::Consecuencias.at(consecuencia) +
                "|" + MatrizRiesgo::Riesgos.at(matrizRiesgo.riesgoReferencia(frecuencia, consecuencia)) +
                "|\n"
             );
        }
        tabla.append("\n\n\n\n");
    }

    delete initiators;

    return tabla;
}

QString Resumen::drawMediumRiskInitiator()
{
    QString tabla;

    QSqlQuery *initiators = new QSqlQuery();
    initiators->prepare(R"""(SELECT codigo,nombre,frecuencia,consecuencia,riesgo FROM suceso_iniciador WHERE riesgo=:risk AND completado=1)""");

    initiators->bindValue(":risk", 1);
    if (initiators->exec()) {
        int frecuencia, consecuencia;
        bool cabeceraMostrada = false;

        while(initiators->next()) {
            if (!cabeceraMostrada) {
                tabla.append(this->drawRiskInitiatorTableHeader("medio (RM)"));
                cabeceraMostrada = true;
            }
            frecuencia = initiators->value(2).toInt();
            consecuencia = initiators->value(3).toInt();
            //qDebug() << matrizRiesgo.riesgoReferencia(frecuencia, consecuencia);

            tabla.append(
                "|" + initiators->value(0).toString() +
                "|" + initiators->value(1).toString() +
                "|" + MatrizRiesgo::Frecuencias.at(frecuencia) +
                "|" + MatrizRiesgo::Consecuencias.at(consecuencia) +
                "|" + MatrizRiesgo::Riesgos.at(matrizRiesgo.riesgoReferencia(frecuencia, consecuencia)) +
                "|\n"
             );
        }
        tabla.append("\n\n\n\n");
    }

    delete initiators;

    return tabla;
}

QString Resumen::drawRiskInitiatorTableHeader(QString title)
{
    return QString("# Iniciadores con riesgo ") + title + QString("\n\n") +
           QString("|Código|Título|Frecuencia|Consecuencia|Riesgo referencia|\n") +
           QString("|------|------|----------|------------|-----------------|\n");
}

void Resumen::drawSummaryChart()
{
    // Chart RMA
    auto *rmaSerie = new QBarSet("RMA");
    for(auto s : summary) {
        *rmaSerie << s.rma;
        rmaSerie->setColor(Styles::RiesgoMuyAlto); // apm
    }
    auto *raSerie = new QBarSet("RA");
    for(auto s : summary) {
        *raSerie << s.ra;
        raSerie->setColor(Styles::RiesgoAlto); // apm
    }
    auto *rmSerie = new QBarSet("RM");
    for(auto s : summary) {
        *rmSerie << s.rm;
        rmSerie->setColor(Styles::RiesgoMedio); //apm
    }
    auto *rbSerie = new QBarSet("RB");
    for(auto s : summary) {
        *rbSerie << s.rb;
        rbSerie->setColor(Styles::RiesgoBajo); //apm
    }

    QBarSeries *series = new QBarSeries();
    series->append(rbSerie);
    series->append(rmSerie);
    series->append(raSerie);
    series->append(rmaSerie);

    // Set the width of the bars in the RB set
    series->setBarWidth(0.9);

    QChart *chart = new QChart();
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->setTitle(tr("DISTRIBUCIÓN DEL RIESGO POR ETAPAS"));
    chart->setAnimationOptions(QChart::SeriesAnimations);


    QStringList categories;
    for(auto s : etapasDb){
        categories << s.codigo;
    }
    //categories << "INS" << "ACE" << "MAN" << "DEC" << "LOC" << "DEL" << "PLA" << "VER" << "SIN" << "TTD";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,30);
    axisY->setTickCount(7);
    axisY->setLabelFormat("%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    riskChartView = new QChartView(chart);
    riskChartView->setRenderHint(QPainter::Antialiasing);
    riskChartView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QVBoxLayout *chartRMALayout = new QVBoxLayout;
    chartRMALayout->addWidget(riskChartView, 0);

    ui->summaryRMAChartsWidget->setLayout(chartRMALayout);
}

void Resumen::drawFrequencyChart()
{
    // Count frequency distribution for base and calculated values
    int freqBase[4] = {0, 0, 0, 0};  // Very low, Low, Medium, High
    int freqCalc[4] = {0, 0, 0, 0};
    
    // Iterate through all events to count frequencies
    for (const auto &stagePair : *events) {
        for (const auto &event : stagePair.second) {
            // Count base frequency
            if (event.frecuencia >= 0 && event.frecuencia <= 3) {
                freqBase[event.frecuencia]++;
            }
            // Count calculated frequency
            if (event.frecuenciaCalculada >= 0 && event.frecuenciaCalculada <= 3) {
                freqCalc[event.frecuenciaCalculada]++;
            }
        }
    }
    
    // Define lighter colors for base values (approximately 40% lighter)
    QColor lightGray("#CFCFCF");    // Lighter version of RiesgoBajo (#A6A6A6)
    QColor lightGreen("#99D699");   // Lighter version of RiesgoMedio (#00B050)
    QColor lightYellow("#FFE899");  // Lighter version of RiesgoAlto (#FFD966)
    QColor lightRed("#FFB3B3");     // Lighter version of RiesgoMuyAlto (#FF0000)
    
    QList<QColor> baseColors = {lightGray, lightGreen, lightYellow, lightRed};
    QList<QColor> calcColors = {Styles::RiesgoBajo, Styles::RiesgoMedio, Styles::RiesgoAlto, Styles::RiesgoMuyAlto};
    
    QStringList categoryNames;
    categoryNames << tr("Muy Baja") << tr("Baja") << tr("Media") << tr("Alta");
    
    // Create separate bar sets for each category (4 categories x 2 types = 8 sets)
    // This allows each bar to have its own color
    QBarSeries *series = new QBarSeries();
    
    for (int i = 0; i < 4; i++) {
        // Base value bar set for this category
        auto *baseSet = new QBarSet(tr("Base ") + categoryNames[i]);
        for (int j = 0; j < 4; j++) {
            if (j == i) {
                *baseSet << freqBase[i];
            } else {
                *baseSet << 0;  // Zero for other categories
            }
        }
        baseSet->setColor(baseColors[i]);
        series->append(baseSet);
        
        // Calculated value bar set for this category
        auto *calcSet = new QBarSet(tr("Calc ") + categoryNames[i]);
        for (int j = 0; j < 4; j++) {
            if (j == i) {
                *calcSet << freqCalc[i];
            } else {
                *calcSet << 0;  // Zero for other categories
            }
        }
        calcSet->setColor(calcColors[i]);
        series->append(calcSet);
    }
    
    series->setBarWidth(0.9);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("DISTRIBUCIÓN POR FRECUENCIA"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    // Show legend but it will have all 8 sets - we'll customize this
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // X axis with frequency categories
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categoryNames);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    // Y axis
    QValueAxis *axisY = new QValueAxis();
    int maxFreq = 0;
    for (int i = 0; i < 4; i++) {
        maxFreq = qMax(maxFreq, qMax(freqBase[i], freqCalc[i]));
    }
    axisY->setRange(0, maxFreq + 5);
    axisY->setTickCount(6);
    axisY->setLabelFormat("%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    frequencyChartView = new QChartView(chart);
    frequencyChartView->setRenderHint(QPainter::Antialiasing);
    frequencyChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QVBoxLayout *chartLayout = new QVBoxLayout;
    chartLayout->addWidget(frequencyChartView);
    
    ui->frequencyChartsWidget->setLayout(chartLayout);
}

void Resumen::drawConsequenceChart()
{
    // Count consequence distribution for base and calculated values
    int consBase[4] = {0, 0, 0, 0};  // Low, Medium, High, Very High
    int consCalc[4] = {0, 0, 0, 0};
    
    // Iterate through all events to count consequences
    for (const auto &stagePair : *events) {
        for (const auto &event : stagePair.second) {
            // Count base consequence
            if (event.consecuencia >= 0 && event.consecuencia <= 3) {
                consBase[event.consecuencia]++;
            }
            // Count calculated consequence
            if (event.consecuenciaCalculada >= 0 && event.consecuenciaCalculada <= 3) {
                consCalc[event.consecuenciaCalculada]++;
            }
        }
    }
    
    // Define lighter colors for base values (approximately 40% lighter)
    QColor lightGray("#CFCFCF");    // Lighter version of RiesgoBajo (#A6A6A6)
    QColor lightGreen("#99D699");   // Lighter version of RiesgoMedio (#00B050)
    QColor lightYellow("#FFE899");  // Lighter version of RiesgoAlto (#FFD966)
    QColor lightRed("#FFB3B3");     // Lighter version of RiesgoMuyAlto (#FF0000)
    
    QList<QColor> baseColors = {lightGray, lightGreen, lightYellow, lightRed};
    QList<QColor> calcColors = {Styles::RiesgoBajo, Styles::RiesgoMedio, Styles::RiesgoAlto, Styles::RiesgoMuyAlto};
    
    QStringList categoryNames;
    categoryNames << tr("Bajas") << tr("Medias") << tr("Altas") << tr("Muy Altas");
    
    // Create separate bar sets for each category (4 categories x 2 types = 8 sets)
    // This allows each bar to have its own color
    QBarSeries *series = new QBarSeries();
    
    for (int i = 0; i < 4; i++) {
        // Base value bar set for this category
        auto *baseSet = new QBarSet(tr("Base ") + categoryNames[i]);
        for (int j = 0; j < 4; j++) {
            if (j == i) {
                *baseSet << consBase[i];
            } else {
                *baseSet << 0;  // Zero for other categories
            }
        }
        baseSet->setColor(baseColors[i]);
        series->append(baseSet);
        
        // Calculated value bar set for this category
        auto *calcSet = new QBarSet(tr("Calc ") + categoryNames[i]);
        for (int j = 0; j < 4; j++) {
            if (j == i) {
                *calcSet << consCalc[i];
            } else {
                *calcSet << 0;  // Zero for other categories
            }
        }
        calcSet->setColor(calcColors[i]);
        series->append(calcSet);
    }
    
    series->setBarWidth(0.9);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("DISTRIBUCIÓN POR CONSECUENCIA"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    // Show legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // X axis with consequence categories
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categoryNames);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    // Y axis
    QValueAxis *axisY = new QValueAxis();
    int maxCons = 0;
    for (int i = 0; i < 4; i++) {
        maxCons = qMax(maxCons, qMax(consBase[i], consCalc[i]));
    }
    axisY->setRange(0, maxCons + 5);
    axisY->setTickCount(6);
    axisY->setLabelFormat("%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    consequenceChartView = new QChartView(chart);
    consequenceChartView->setRenderHint(QPainter::Antialiasing);
    consequenceChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QVBoxLayout *chartLayout = new QVBoxLayout;
    chartLayout->addWidget(consequenceChartView);
    
    ui->consequenceChartsWidget->setLayout(chartLayout);
}

void Resumen::on_btRiskSourceText_clicked()
{
    if (ui->tIniciadoresMayorRiesgo->textFormat() == Qt::MarkdownText) {
        ui->tIniciadoresMayorRiesgo->setTextFormat(Qt::PlainText);
        ui->btRiskSourceText->setText(tr("Ver renderizado"));
    } else {
        ui->tIniciadoresMayorRiesgo->setTextFormat(Qt::MarkdownText);
        ui->btRiskSourceText->setText(tr("Ver Markdown"));
    }
}

void Resumen::exportChartToPNG(QChartView *chartView, const QString &defaultFileName)
{
    if (!chartView) {
        QMessageBox::warning(this, tr("Error"), tr("No hay gráfica disponible para exportar"));
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Guardar gráfica como imagen"),
        defaultFileName,
        tr("Imágenes PNG (*.png)")
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Ensure .png extension
    if (!fileName.endsWith(".png", Qt::CaseInsensitive)) {
        fileName += ".png";
    }
    
    // Create a high-resolution image for better quality
    QSize size(1200, 800);  // High resolution for report quality
    QPixmap pixmap(size);
    pixmap.fill(Qt::white);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    chartView->render(&painter, QRectF(QPointF(0,0), size), chartView->rect());
    painter.end();
    
    if (pixmap.save(fileName)) {
        QMessageBox::information(this, tr("Exportar gráfica"), 
            tr("Gráfica exportada correctamente a:\n%1").arg(fileName));
    } else {
        QMessageBox::warning(this, tr("Error"), 
            tr("No se pudo guardar la imagen en:\n%1").arg(fileName));
    }
}

void Resumen::on_btExportFrequencyChart_clicked()
{
    exportChartToPNG(frequencyChartView, "distribucion_frecuencia.png");
}

void Resumen::on_btExportConsequenceChart_clicked()
{
    exportChartToPNG(consequenceChartView, "distribucion_consecuencia.png");
}

void Resumen::on_btExportRiskChart_clicked()
{
    exportChartToPNG(riskChartView, "distribucion_riesgo_etapas.png");
}
