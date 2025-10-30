#include "stageadapter.h"
#include "../mainwindow.h"
#include "../dbelementstage.h"
#include "../dbelementsubstage.h"
#include "../etapas.h"
#include "../subetapas.h"
#include <QDebug>

StageAdapter::StageAdapter(MainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow)
{
}

QVariantList StageAdapter::getAllStages()
{
    QVariantList result;
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> stages;
    
    try {
        dbStage.list(stages);
        qDebug() << "StageAdapter::getAllStages() - Retrieved" << stages.size() << "stages";
        
        for (const auto &stage : stages) {
            result.append(stageToVariant(stage));
        }
    } catch (const Database::DbException& exception) {
        qWarning() << "StageAdapter::getAllStages() - Error:" << exception.what();
    } catch (...) {
        qWarning() << "StageAdapter::getAllStages() - Unknown error";
    }
    
    return result;
}

QVariantList StageAdapter::getAllSubstages()
{
    QVariantList result;
    QList<SubetapaDefinition> substages;
    
    try {
        Database::DbElementSubstage::listAll(&substages);
        qDebug() << "StageAdapter::getAllSubstages() - Retrieved" << substages.size() << "substages";
        
        for (const auto &substage : substages) {
            result.append(substageToVariant(substage));
        }
    } catch (...) {
        qWarning() << "StageAdapter::getAllSubstages() - Error retrieving substages";
    }
    
    return result;
}

QVariantList StageAdapter::getSubstagesByStage(int stageId)
{
    QVariantList result;
    QList<SubetapaDefinition> substages;
    
    try {
        Database::DbElementSubstage dbSubstage(stageId);
        dbSubstage.list(substages);
        qDebug() << "StageAdapter::getSubstagesByStage(" << stageId << ") - Retrieved" << substages.size() << "substages";
        
        for (const auto &substage : substages) {
            result.append(substageToVariant(substage));
        }
    } catch (...) {
        qWarning() << "StageAdapter::getSubstagesByStage() - Error retrieving substages for stage" << stageId;
    }
    
    return result;
}

QVariantMap StageAdapter::getStagesWithSubstages()
{
    QVariantMap result;
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> stages;
    
    try {
        dbStage.list(stages);
        qDebug() << "StageAdapter::getStagesWithSubstages() - Retrieved" << stages.size() << "stages";
        
        for (const auto &stage : stages) {
            QVariantMap stageData = stageToVariant(stage);
            stageData["substages"] = getSubstagesByStage(stage.id);
            result[stage.nombre] = stageData;
        }
    } catch (const Database::DbException& exception) {
        qWarning() << "StageAdapter::getStagesWithSubstages() - Error:" << exception.what();
    } catch (...) {
        qWarning() << "StageAdapter::getStagesWithSubstages() - Unknown error";
    }
    
    return result;
}

QVariantMap StageAdapter::getStageById(int stageId)
{
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> stages;
    
    try {
        dbStage.list(stages);
        
        for (const auto &stage : stages) {
            if (stage.id == stageId) {
                qDebug() << "StageAdapter::getStageById(" << stageId << ") - Found stage:" << stage.nombre;
                return stageToVariant(stage);
            }
        }
        
        qWarning() << "StageAdapter::getStageById(" << stageId << ") - Stage not found";
    } catch (const Database::DbException& exception) {
        qWarning() << "StageAdapter::getStageById() - Error:" << exception.what();
    } catch (...) {
        qWarning() << "StageAdapter::getStageById() - Unknown error";
    }
    
    return QVariantMap();
}

QVariantMap StageAdapter::getSubstageById(int substageId)
{
    QList<SubetapaDefinition> substages;
    
    try {
        Database::DbElementSubstage::listAll(&substages);
        
        for (const auto &substage : substages) {
            if (substage.id == substageId) {
                qDebug() << "StageAdapter::getSubstageById(" << substageId << ") - Found substage:" << substage.nombre;
                return substageToVariant(substage);
            }
        }
        
        qWarning() << "StageAdapter::getSubstageById(" << substageId << ") - Substage not found";
    } catch (...) {
        qWarning() << "StageAdapter::getSubstageById() - Error retrieving substage";
    }
    
    return QVariantMap();
}

QVariantMap StageAdapter::countSubstagesByStage()
{
    QVariantMap result;
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> stages;
    
    try {
        dbStage.list(stages);
        qDebug() << "StageAdapter::countSubstagesByStage() - Counting substages for" << stages.size() << "stages";
        
        for (const auto &stage : stages) {
            QVariantList substages = getSubstagesByStage(stage.id);
            result[stage.nombre] = substages.size();
        }
    } catch (const Database::DbException& exception) {
        qWarning() << "StageAdapter::countSubstagesByStage() - Error:" << exception.what();
    } catch (...) {
        qWarning() << "StageAdapter::countSubstagesByStage() - Unknown error";
    }
    
    return result;
}

bool StageAdapter::updateStage(int stageId, const QVariantMap &properties)
{
    try {
        // First, get the current stage
        Database::DbElementStage dbStage;
        QList<EtapaDefinition> stages;
        dbStage.list(stages, true); // Get all stages including negative IDs
        
        EtapaDefinition *targetStage = nullptr;
        for (auto &stage : stages) {
            if (stage.id == stageId) {
                targetStage = &stage;
                break;
            }
        }
        
        if (!targetStage) {
            qWarning() << "StageAdapter::updateStage() - Stage" << stageId << "not found";
            return false;
        }
        
        // Update properties
        if (properties.contains("nombre")) {
            targetStage->nombre = properties["nombre"].toString();
        }
        if (properties.contains("descripcion")) {
            targetStage->descripcion = properties["descripcion"].toString();
        }
        if (properties.contains("codigo")) {
            targetStage->codigo = properties["codigo"].toString();
        }
        if (properties.contains("orden")) {
            targetStage->orden = properties["orden"].toInt();
        }
        
        // Mark as modified and update in database
        targetStage->modificado = true;
        QList<EtapaDefinition> stagesToUpdate;
        stagesToUpdate.append(*targetStage);
        dbStage.update(stagesToUpdate, true);
        
        qDebug() << "StageAdapter::updateStage() - Successfully updated stage" << stageId;
        return true;
        
    } catch (const QString &error) {
        qWarning() << "StageAdapter::updateStage() - Error:" << error;
        return false;
    } catch (...) {
        qWarning() << "StageAdapter::updateStage() - Unknown error";
        return false;
    }
}

QVariantMap StageAdapter::stageToVariant(const EtapaDefinition &stage)
{
    QVariantMap result;
    
    result["id"] = stage.id;
    result["nombre"] = stage.nombre;
    result["codigo"] = stage.codigo;
    result["descripcion"] = stage.descripcion;
    result["orden"] = stage.orden;
    
    return result;
}

QVariantMap StageAdapter::substageToVariant(const SubetapaDefinition &substage)
{
    QVariantMap result;
    
    result["id"] = substage.id;
    result["nombre"] = substage.nombre;
    result["descripcion"] = substage.descripcion;
    result["etapaId"] = substage.etapaId;
    result["activo"] = substage.activo;
    result["orden"] = substage.orden;
    
    return result;
}
