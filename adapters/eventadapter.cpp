#include "eventadapter.h"
#include "../mainwindow.h"
#include "../dbevent.h"
#include "../dbdefense.h"
#include "../defensecalc.h"
#include "../dbelementstage.h"
#include "../dbelementsubstage.h"
#include "../etapas.h"
#include "../subetapas.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

EventAdapter::EventAdapter(MainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow)
{
}

QVariantList EventAdapter::getAllEvents()
{
    QVariantList result;
    Database::DbEvent dbEvent;
    QList<EventDefinition> events;
    
    qDebug() << "EventAdapter::getAllEvents() - Starting";
    
    // Get all events (using stage -1 to get all)
    dbEvent.list(events, -1);
    
    qDebug() << "EventAdapter::getAllEvents() - Retrieved" << events.size() << "events";
    
    for (const auto &event : events) {
        result.append(eventToVariant(event));
    }
    
    return result;
}

QVariantList EventAdapter::getEventsByStage(int stageId, int substageId)
{
    QVariantList result;
    Database::DbEvent dbEvent;
    QList<EventDefinition> events;
    
    qDebug() << "EventAdapter::getEventsByStage(" << stageId << "," << substageId << ") - Starting";
    
    dbEvent.list(events, stageId, substageId);
    
    qDebug() << "EventAdapter::getEventsByStage() - Retrieved" << events.size() << "events";
    
    for (const auto &event : events) {
        result.append(eventToVariant(event));
    }
    
    return result;
}

QVariantMap EventAdapter::getEventsByStageGrouped()
{
    QVariantMap result;
    
    // Get all events at once instead of querying per stage
    QVariantList allEvents = getAllEvents();
    
    // Group events by stage name
    for (const auto &eventVar : allEvents) {
        QVariantMap event = eventVar.toMap();
        QString stageName = event["etapa"].toString();
        
        if (!result.contains(stageName)) {
            result[stageName] = QVariantList();
        }
        
        QVariantList list = result[stageName].toList();
        list.append(event);
        result[stageName] = list;
    }
    
    return result;
}

QVariantMap EventAdapter::countEventsByStage()
{
    QVariantMap result;
    
    // Get all events at once instead of querying per stage
    QVariantList allEvents = getAllEvents();
    
    // Initialize counts for all stages
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> stages;
    dbStage.list(stages, true); // Get all stages including negative IDs
    
    for (const auto &stage : stages) {
        result[stage.nombre] = 0;
    }
    
    // Count events by stage name
    for (const auto &eventVar : allEvents) {
        QVariantMap event = eventVar.toMap();
        QString stageName = event["etapa"].toString();
        result[stageName] = result[stageName].toInt() + 1;
    }
    
    return result;
}

QVariantMap EventAdapter::getEventsByConsequence()
{
    QVariantMap result;
    QVariantList allEvents = getAllEvents();
    
    for (const auto &eventVar : allEvents) {
        QVariantMap event = eventVar.toMap();
        int consecuencia = event["consecuencia"].toInt();
        QString consecuenciaStr = QString("Consecuencia %1").arg(consecuencia);
        
        if (!result.contains(consecuenciaStr)) {
            result[consecuenciaStr] = QVariantList();
        }
        
        QVariantList list = result[consecuenciaStr].toList();
        list.append(event);
        result[consecuenciaStr] = list;
    }
    
    return result;
}

QVariantMap EventAdapter::getEventsByRisk()
{
    QVariantMap result;
    QVariantList allEvents = getAllEvents();
    
    for (const auto &eventVar : allEvents) {
        QVariantMap event = eventVar.toMap();
        int riesgo = event["riesgo"].toInt();
        QString riesgoStr = MatrizRiesgo::Riesgos.count(riesgo) ? 
            MatrizRiesgo::Riesgos.at(riesgo) : QString("Riesgo %1").arg(riesgo);
        
        if (!result.contains(riesgoStr)) {
            result[riesgoStr] = QVariantList();
        }
        
        QVariantList list = result[riesgoStr].toList();
        list.append(event);
        result[riesgoStr] = list;
    }
    
    return result;
}

QVariantMap EventAdapter::getEventStatistics()
{
    QVariantMap result;
    QVariantList allEvents = getAllEvents();
    
    result["totalEvents"] = allEvents.size();
    result["eventsByStage"] = countEventsByStage();
    
    // Count by risk levels
    QVariantMap riskCounts;
    QVariantMap consequenceCounts;
    QVariantMap frequencyCounts;
    int humanErrorCount = 0;
    
    for (const auto &eventVar : allEvents) {
        QVariantMap event = eventVar.toMap();
        
        // Risk counts
        int riesgo = event["riesgo"].toInt();
        QString riesgoStr = MatrizRiesgo::Riesgos.count(riesgo) ? 
            MatrizRiesgo::Riesgos.at(riesgo) : QString("Riesgo %1").arg(riesgo);
        riskCounts[riesgoStr] = riskCounts[riesgoStr].toInt() + 1;
        
        // Consequence counts
        int consecuencia = event["consecuencia"].toInt();
        QString consecuenciaStr = QString("Consecuencia %1").arg(consecuencia);
        consequenceCounts[consecuenciaStr] = consequenceCounts[consecuenciaStr].toInt() + 1;
        
        // Frequency counts
        int frecuencia = event["frecuencia"].toInt();
        QString frecuenciaStr = QString("Frecuencia %1").arg(frecuencia);
        frequencyCounts[frecuenciaStr] = frequencyCounts[frecuenciaStr].toInt() + 1;
        
        // Human error count
        if (event["errorHumano"].toBool()) {
            humanErrorCount++;
        }
    }
    
    result["eventsByRisk"] = riskCounts;
    result["eventsByConsequence"] = consequenceCounts;
    result["eventsByFrequency"] = frequencyCounts;
    result["humanErrorEvents"] = humanErrorCount;
    result["nonHumanErrorEvents"] = allEvents.size() - humanErrorCount;
    
    return result;
}

QVariantMap EventAdapter::eventToVariant(const EventDefinition &event)
{
    QVariantMap result;
    
    result["id"] = event.id;
    result["codigo"] = event.codigo;
    result["nombre"] = event.nombre;
    result["descripcion"] = event.descripcion;
    result["etapa"] = event.etapa.nombre;
    result["etapaId"] = event.etapa.id;
    result["subetapa"] = event.subetapa.nombre;
    result["subetapaId"] = event.subetapa.id;
    
    // Base values from database
    result["frecuenciaBase"] = event.frecuencia;
    result["consecuenciaBase"] = event.consecuencia;
    result["errorHumano"] = event.errorHumano;
    result["base"] = event.base;
    
    // Calculate base risk (assumes maximum probability = 3)
    int riesgoBase = riskMatrix.riesgo(3, event.frecuencia, event.consecuencia);
    result["riesgoBase"] = riesgoBase;
    result["riesgoBaseTexto"] = MatrizRiesgo::Riesgos.count(riesgoBase) ? 
        MatrizRiesgo::Riesgos.at(riesgoBase) : QString("Riesgo %1").arg(riesgoBase);
    
    // Get defenses for this event to calculate adjusted values
    Database::DbDefense dbDefense;
    
    // Get defenses by type using groupByHardiness (which already filters by active)
    auto freqReducerGroups = dbDefense.groupByHardiness(event.id, DefenseType::FrecuencyReducer, true);
    auto barrierGroups = dbDefense.groupByHardiness(event.id, DefenseType::Barrier, true);
    auto consReducerGroups = dbDefense.groupByHardiness(event.id, DefenseType::ConsequenceReducer, true);
    
    // Calculate reducers using DefenseCalc methods
    std::vector<std::pair<int, int>> freqInfo = freqReducerGroups;
    std::vector<std::pair<int, int>> barrierInfo = barrierGroups;
    std::vector<std::pair<int, int>> consInfo = consReducerGroups;
    
    // Ensure we have entries for all hardiness levels (0-3) for consistency
    int freqReducers[4] = {0, 0, 0, 0};
    int barriers[4] = {0, 0, 0, 0};
    int consReducers[4] = {0, 0, 0, 0};
    for (const auto &pair : freqReducerGroups) {
        if (pair.first >= 0 && pair.first <= 3) freqReducers[pair.first] = pair.second;
    }
    for (const auto &pair : barrierGroups) {
        if (pair.first >= 0 && pair.first <= 3) barriers[pair.first] = pair.second;
    }
    for (const auto &pair : consReducerGroups) {
        if (pair.first >= 0 && pair.first <= 3) consReducers[pair.first] = pair.second;
    }
    freqInfo.clear(); barrierInfo.clear(); consInfo.clear();
    for (int i = 0; i < 4; i++) {
        freqInfo.push_back(std::make_pair(i, freqReducers[i]));
        barrierInfo.push_back(std::make_pair(i, barriers[i]));
        consInfo.push_back(std::make_pair(i, consReducers[i]));
    }
    
    int freqReductor = DefenseCalc::getFrecuencyReducer(freqInfo);
    int barrierReductor = DefenseCalc::getBarriers(barrierInfo);
    int consReductor = DefenseCalc::getConsequencyReducer(consInfo);
    
    // Calculate adjusted values
    int frecuenciaCalculada = DefenseCalc::calcFrecuencyLevel(event.frecuencia, event.errorHumano, freqReductor);
    int probabilidadCalculada = DefenseCalc::calcProbabilityLevel(barrierReductor);
    int consecuenciaCalculada = DefenseCalc::calcConsequencyLevel(event.consecuencia, consReductor);
    
    result["frecuencia"] = frecuenciaCalculada;
    result["probabilidad"] = probabilidadCalculada;
    result["consecuencia"] = consecuenciaCalculada;
    
    // Add text literals for frequency, probability, and consequence
    
    result["frecuenciaTexto"] = MatrizRiesgo::Frecuencias.count(frecuenciaCalculada) ? 
        MatrizRiesgo::Frecuencias.at(frecuenciaCalculada) : QString("F%1").arg(frecuenciaCalculada);
    result["probabilidadTexto"] = MatrizRiesgo::Probabilidades.count(probabilidadCalculada) ? 
        MatrizRiesgo::Probabilidades.at(probabilidadCalculada) : QString("P%1").arg(probabilidadCalculada);
    result["consecuenciaTexto"] = MatrizRiesgo::Consecuencias.count(consecuenciaCalculada) ? 
        MatrizRiesgo::Consecuencias.at(consecuenciaCalculada) : QString("C%1").arg(consecuenciaCalculada);
    
    // Calculate risk using the risk matrix with calculated values
    int riesgo = riskMatrix.riesgo(probabilidadCalculada, frecuenciaCalculada, consecuenciaCalculada);
    result["riesgo"] = riesgo;
    result["riesgoTexto"] = MatrizRiesgo::Riesgos.count(riesgo) ? 
        MatrizRiesgo::Riesgos.at(riesgo) : QString("Riesgo %1").arg(riesgo);
    
    return result;
}

bool EventAdapter::updateEvent(int eventId, const QVariantMap &properties)
{
    try {
        // First, get the current event
        Database::DbEvent dbEvent;
        QList<EventDefinition> events;
        dbEvent.list(events, -1); // Get all events
        
        EventDefinition *targetEvent = nullptr;
        for (auto &event : events) {
            if (event.id == eventId) {
                targetEvent = &event;
                break;
            }
        }
        
        if (!targetEvent) {
            qWarning() << "EventAdapter::updateEvent() - Event" << eventId << "not found";
            return false;
        }
        
        // Update properties
        if (properties.contains("nombre")) {
            targetEvent->nombre = properties["nombre"].toString();
        }
        if (properties.contains("descripcion")) {
            targetEvent->descripcion = properties["descripcion"].toString();
        }
        if (properties.contains("codigo")) {
            targetEvent->codigo = properties["codigo"].toString();
        }
        if (properties.contains("frecuencia")) {
            targetEvent->frecuencia = properties["frecuencia"].toInt();
        }
        if (properties.contains("consecuencia")) {
            targetEvent->consecuencia = properties["consecuencia"].toInt();
        }
        if (properties.contains("etapaId")) {
            targetEvent->etapa.id = properties["etapaId"].toInt();
        }
        if (properties.contains("subetapaId")) {
            targetEvent->subetapa.id = properties["subetapaId"].toInt();
        }
        if (properties.contains("errorHumano")) {
            targetEvent->errorHumano = properties["errorHumano"].toBool();
        }
        
        // Update in database
        Database::DbEvent::updateElement(*targetEvent);
        
        // Handle completado separately if provided
        if (properties.contains("completado")) {
            bool completado = properties["completado"].toBool();
            Database::DbEvent::setCompleted(eventId, completado, 0); // riesgo will be recalculated
        }
        
        qDebug() << "EventAdapter::updateEvent() - Successfully updated event" << eventId;
        return true;
        
    } catch (const QString &error) {
        qWarning() << "EventAdapter::updateEvent() - Error:" << error;
        return false;
    } catch (...) {
        qWarning() << "EventAdapter::updateEvent() - Unknown error";
        return false;
    }
}

int EventAdapter::setAllEventsApplicable()
{
    try {
        QSqlQuery query;
        query.prepare("UPDATE suceso_iniciador SET completado=1");
        
        if (!query.exec()) {
            qWarning() << "EventAdapter::setAllEventsApplicable() - Error:" << query.lastError();
            return 0;
        }
        
        int count = query.numRowsAffected();
        qDebug() << "EventAdapter::setAllEventsApplicable() - Updated" << count << "events";
        return count;
        
    } catch (...) {
        qWarning() << "EventAdapter::setAllEventsApplicable() - Unknown error";
        return 0;
    }
}

int EventAdapter::setAllEventsNotApplicable()
{
    try {
        QSqlQuery query;
        query.prepare("UPDATE suceso_iniciador SET completado=0");
        
        if (!query.exec()) {
            qWarning() << "EventAdapter::setAllEventsNotApplicable() - Error:" << query.lastError();
            return 0;
        }
        
        int count = query.numRowsAffected();
        qDebug() << "EventAdapter::setAllEventsNotApplicable() - Updated" << count << "events";
        return count;
        
    } catch (...) {
        qWarning() << "EventAdapter::setAllEventsNotApplicable() - Unknown error";
        return 0;
    }
}

int EventAdapter::searchReplaceInDescriptions(const QString &searchTerm, const QString &replaceTerm, bool caseSensitive)
{
    try {
        Database::DbEvent dbEvent;
        QList<EventDefinition> events;
        dbEvent.list(events, -1); // Get all events
        
        int count = 0;
        Qt::CaseSensitivity cs = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
        
        for (auto &event : events) {
            if (event.descripcion.contains(searchTerm, cs)) {
                // Replace text
                event.descripcion.replace(searchTerm, replaceTerm, cs);
                
                // Update in database
                Database::DbEvent::updateElement(event);
                count++;
            }
        }
        
        qDebug() << "EventAdapter::searchReplaceInDescriptions() - Updated" << count << "events";
        return count;
        
    } catch (const QString &error) {
        qWarning() << "EventAdapter::searchReplaceInDescriptions() - Error:" << error;
        return 0;
    } catch (...) {
        qWarning() << "EventAdapter::searchReplaceInDescriptions() - Unknown error";
        return 0;
    }
}

QString EventAdapter::getStageName(int stageId)
{
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> stages;
    dbStage.list(stages);
    
    for (const auto &stage : stages) {
        if (stage.id == stageId) {
            return stage.nombre;
        }
    }
    
    return QString("Etapa %1").arg(stageId);
}

QString EventAdapter::getSubstageName(int substageId)
{
    QList<SubetapaDefinition> substages;
    Database::DbElementSubstage::listAll(&substages);
    
    for (const auto &substage : substages) {
        if (substage.id == substageId) {
            return substage.nombre;
        }
    }
    
    return QString("Subetapa %1").arg(substageId);
}
