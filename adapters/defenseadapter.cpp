#include "defenseadapter.h"
#include "../mainwindow.h"
#include "../dbdefense.h"
#include "../dbevent.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DefenseAdapter::DefenseAdapter(MainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow)
{
}

QVariantList DefenseAdapter::getAllDefenses(int eventId)
{
    QVariantList result;
    Database::DbDefense dbDefense;
    
    qDebug() << "DefenseAdapter::getAllDefenses(" << eventId << ") - Starting";
    
    if (eventId > 0) {
        // Get defenses for specific event
        QList<DefenseDefinition> defenses;
        
        // Get all defense types for the event
        dbDefense.list(defenses, eventId, DefenseType::FrecuencyReducer);
        qDebug() << "DefenseAdapter::getAllDefenses() - FrecuencyReducer:" << defenses.size();
        for (const auto &defense : defenses) {
            result.append(defenseToVariant(defense));
        }
        
        defenses.clear();
        dbDefense.list(defenses, eventId, DefenseType::Barrier);
        qDebug() << "DefenseAdapter::getAllDefenses() - Barrier:" << defenses.size();
        for (const auto &defense : defenses) {
            result.append(defenseToVariant(defense));
        }
        
        defenses.clear();
        dbDefense.list(defenses, eventId, DefenseType::ConsequenceReducer);
        qDebug() << "DefenseAdapter::getAllDefenses() - ConsequenceReducer:" << defenses.size();
        for (const auto &defense : defenses) {
            result.append(defenseToVariant(defense));
        }
    } else {
        // Get all defenses for all events
        Database::DbEvent dbEvent;
        QList<EventDefinition> events;
        dbEvent.list(events, -1); // Get all events
        
        qDebug() << "DefenseAdapter::getAllDefenses() - Processing defenses for" << events.size() << "events";
        
        for (const auto &event : events) {
            QVariantList eventDefenses = getAllDefenses(event.id);
            result.append(eventDefenses);
        }
    }
    
    return result;
}

QVariantList DefenseAdapter::getDefensesByType(int type, int eventId)
{
    QVariantList result;
    
    if (type < 0 || type > 2) {
        return result; // Invalid type
    }
    
    DefenseType defenseType = static_cast<DefenseType>(type);
    Database::DbDefense dbDefense;
    
    if (eventId > 0) {
        QList<DefenseDefinition> defenses;
        dbDefense.list(defenses, eventId, defenseType);
        
        for (const auto &defense : defenses) {
            result.append(defenseToVariant(defense));
        }
    } else {
        // Get defenses of this type for all events
        Database::DbEvent dbEvent;
        QList<EventDefinition> events;
        dbEvent.list(events, -1);
        
        for (const auto &event : events) {
            QList<DefenseDefinition> defenses;
            dbDefense.list(defenses, event.id, defenseType);
            
            for (const auto &defense : defenses) {
                result.append(defenseToVariant(defense));
            }
        }
    }
    
    return result;
}

QVariantList DefenseAdapter::getBarriers(int eventId)
{
    return getDefensesByType(static_cast<int>(DefenseType::Barrier), eventId);
}

QVariantList DefenseAdapter::getFrequencyReducers(int eventId)
{
    return getDefensesByType(static_cast<int>(DefenseType::FrecuencyReducer), eventId);
}

QVariantList DefenseAdapter::getConsequenceReducers(int eventId)
{
    return getDefensesByType(static_cast<int>(DefenseType::ConsequenceReducer), eventId);
}

QVariantMap DefenseAdapter::getDefensesByTypeGrouped(int eventId)
{
    QVariantMap result;
    
    result["Reductores de Frecuencia"] = getFrequencyReducers(eventId);
    result["Barreras"] = getBarriers(eventId);
    result["Reductores de Consecuencia"] = getConsequenceReducers(eventId);
    
    return result;
}

QVariantMap DefenseAdapter::getDefensesByHardiness(int eventId)
{
    QVariantMap result;
    QVariantList allDefenses = getAllDefenses(eventId);
    
    for (const auto &defenseVar : allDefenses) {
        QVariantMap defense = defenseVar.toMap();
        int hardiness = defense["hardiness"].toInt();
        QString hardinessStr = getHardinessName(hardiness);
        
        if (!result.contains(hardinessStr)) {
            result[hardinessStr] = QVariantList();
        }
        
        QVariantList list = result[hardinessStr].toList();
        list.append(defense);
        result[hardinessStr] = list;
    }
    
    return result;
}

int DefenseAdapter::setHumanBarriersNotApplicable()
{
    int modifiedCount = 0;
    QVariantList barriers = getBarriers(); // Get all barriers
    
    for (const auto &barrierVar : barriers) {
        QVariantMap barrier = barrierVar.toMap();
        int hardiness = barrier["hardiness"].toInt();
        int defenseId = barrier["id"].toInt();
        bool currentlyActive = barrier["active"].toBool();
        
        // Human barriers are those with hardiness >= 3
        if (isHumanDefense(hardiness) && currentlyActive) {
            if (setDefenseActive(defenseId, false)) {
                modifiedCount++;
            }
        }
    }
    
    return modifiedCount;
}

int DefenseAdapter::setNonHumanBarriersApplicable()
{
    int modifiedCount = 0;
    QVariantList barriers = getBarriers(); // Get all barriers
    
    for (const auto &barrierVar : barriers) {
        QVariantMap barrier = barrierVar.toMap();
        int hardiness = barrier["hardiness"].toInt();
        int defenseId = barrier["id"].toInt();
        bool currentlyActive = barrier["active"].toBool();
        
        // Non-human barriers are those with hardiness < 3
        if (!isHumanDefense(hardiness) && !currentlyActive) {
            if (setDefenseActive(defenseId, true)) {
                modifiedCount++;
            }
        }
    }
    
    return modifiedCount;
}

bool DefenseAdapter::setDefenseActive(int defenseId, bool active)
{
    try {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE defensa SET activo = :active WHERE id = :id");
        updateQuery.bindValue(":active", active);
        updateQuery.bindValue(":id", defenseId);
        
        if (!updateQuery.exec()) {
            qDebug() << "Error updating defense active state:" << updateQuery.lastError().text();
            return false;
        }
        
        return updateQuery.numRowsAffected() > 0;
    } catch (...) {
        return false;
    }
}

QVariantMap DefenseAdapter::getDefenseStatistics()
{
    QVariantMap result;
    QVariantList allDefenses = getAllDefenses();
    
    result["totalDefenses"] = allDefenses.size();
    
    // Count by type
    QVariantMap typeCounts;
    QVariantMap hardinessCounts;
    QVariantMap activecounts;
    int humanDefenseCount = 0;
    int activeDefenseCount = 0;
    
    for (const auto &defenseVar : allDefenses) {
        QVariantMap defense = defenseVar.toMap();
        
        // Type counts
        QString type = defense["typeString"].toString();
        typeCounts[type] = typeCounts[type].toInt() + 1;
        
        // Hardiness counts
        int hardiness = defense["hardiness"].toInt();
        QString hardinessStr = getHardinessName(hardiness);
        hardinessCounts[hardinessStr] = hardinessCounts[hardinessStr].toInt() + 1;
        
        // Active state
        bool active = defense["active"].toBool();
        if (active) {
            activeDefenseCount++;
        }
        
        // Human defense count
        if (isHumanDefense(hardiness)) {
            humanDefenseCount++;
        }
    }
    
    result["defensesByType"] = typeCounts;
    result["defensesByHardiness"] = hardinessCounts;
    result["activeDefenses"] = activeDefenseCount;
    result["inactiveDefenses"] = allDefenses.size() - activeDefenseCount;
    result["humanDefenses"] = humanDefenseCount;
    result["nonHumanDefenses"] = allDefenses.size() - humanDefenseCount;
    
    return result;
}

bool DefenseAdapter::isHumanDefense(int hardiness)
{
    // Based on typical risk analysis, human barriers have hardiness >= 3
    return hardiness >= 3;
}

bool DefenseAdapter::setDefenseHardiness(int defenseId, int hardiness)
{
    try {
        // Validate hardiness level
        if (hardiness < 0 || hardiness > 3) {
            qDebug() << "Invalid hardiness level:" << hardiness << "(must be 0-3)";
            return false;
        }
        
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE defensa SET robustez = :hardiness WHERE id = :id");
        updateQuery.bindValue(":hardiness", hardiness);
        updateQuery.bindValue(":id", defenseId);
        
        if (!updateQuery.exec()) {
            qDebug() << "Error updating defense hardiness:" << updateQuery.lastError().text();
            return false;
        }
        
        return updateQuery.numRowsAffected() > 0;
    } catch (...) {
        return false;
    }
}

bool DefenseAdapter::recalculateRisks()
{
    // Note: This function doesn't actually need to do anything.
    // The risk recalculation happens automatically when Events.getAllEvents()
    // is called, as it queries the database fresh and recalculates risks based
    // on the current state of defenses.
    // We keep this method for API completeness and future enhancements.
    qDebug() << "DefenseAdapter::recalculateRisks() called";
    return true;
}

QVariantMap DefenseAdapter::defenseToVariant(const DefenseDefinition &defense)
{
    QVariantMap result;
    
    result["id"] = defense.id;
    result["code"] = defense.code;
    result["nombre"] = defense.nombre;
    result["descripcion"] = defense.descripcion;
    result["type"] = static_cast<int>(defense.type);
    result["typeString"] = getDefenseTypeName(defense.type);
    result["hardiness"] = defense.hardiness;
    result["hardinessString"] = getHardinessName(defense.hardiness);
    result["eventId"] = defense.event;
    result["orderStageId"] = defense.orderStageId;
    result["orderStage"] = defense.orderStage;
    result["isHuman"] = isHumanDefense(defense.hardiness);
    
    // Use active status directly from DefenseDefinition (now retrieved by DbDefense::list)
    result["active"] = defense.active;
    
    return result;
}

QString DefenseAdapter::getDefenseTypeName(DefenseType type)
{
    switch (type) {
        case DefenseType::FrecuencyReducer:
            return "Reductor de Frecuencia";
        case DefenseType::Barrier:
            return "Barrera";
        case DefenseType::ConsequenceReducer:
            return "Reductor de Consecuencia";
        default:
            return "Desconocido";
    }
}

QString DefenseAdapter::getHardinessName(int hardiness)
{
    if (MatrizRiesgo::Durezas.count(hardiness)) {
        return MatrizRiesgo::Durezas.at(hardiness);
    }
    return QString("Dureza %1").arg(hardiness);
}