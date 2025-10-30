#include "dlgadddefensetoevents.h"
#include "ui_dlgadddefensetoevents.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

DlgAddDefenseToEvents::DlgAddDefenseToEvents(const DefenseDefinition &defense, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddDefenseToEvents),
    defense(defense)
{
    ui->setupUi(this);
    
    // Connect the itemChanged signal
    connect(ui->treeEvents, &QTreeWidget::itemChanged, this, &DlgAddDefenseToEvents::onItemChanged);
    
    loadEventTree();
}

DlgAddDefenseToEvents::~DlgAddDefenseToEvents()
{
    delete ui;
}

void DlgAddDefenseToEvents::loadEventTree()
{
    // Query to get only functional stages (id >= 0), excluding accessory stages
    QSqlQuery qryStages;
    qryStages.prepare("SELECT id, nombre, codigo, orden FROM etapas WHERE activo = 1 AND id >= 0 ORDER BY orden");
    qryStages.exec();
    
    qDebug() << "Loading event tree...";
    
    while (qryStages.next()) {
        int stageId = qryStages.value(0).toInt();
        QString stageName = qryStages.value(1).toString();
        QString stageCode = qryStages.value(2).toString();
        
        qDebug() << "Stage:" << stageId << stageName;
        
        // Create stage item (checkbox will be added later if it has events)
        QTreeWidgetItem *stageItem = new QTreeWidgetItem(ui->treeEvents);
        stageItem->setText(0, QString("%1 - %2").arg(stageCode).arg(stageName));
        stageItem->setData(0, Qt::UserRole, stageId);
        stageItem->setData(0, Qt::UserRole + 1, "stage");
        
        int stageEventCount = 0; // Track if stage has any events
        
        // Query substages for this stage
        QSqlQuery qrySubstages;
        qrySubstages.prepare("SELECT id, nombre, orden FROM subetapas WHERE etapa_id = :stageId AND activo = 1 ORDER BY orden");
        qrySubstages.bindValue(":stageId", stageId);
        qrySubstages.exec();
        
        bool hasSubstages = false;
        while (qrySubstages.next()) {
            hasSubstages = true;
            int substageId = qrySubstages.value(0).toInt();
            QString substageName = qrySubstages.value(1).toString();
            
            qDebug() << "  Substage:" << substageId << substageName;
            
            // Create substage item (checkbox will be added later if it has events)
            QTreeWidgetItem *substageItem = new QTreeWidgetItem(stageItem);
            substageItem->setText(0, substageName);
            substageItem->setData(0, Qt::UserRole, substageId);
            substageItem->setData(0, Qt::UserRole + 1, "substage");
            substageItem->setData(0, Qt::UserRole + 2, stageId);
            
            int substageEventCount = 0; // Track if substage has any events
            
            // Query events for this substage
            QSqlQuery qryEvents;
            qryEvents.prepare("SELECT id, nombre, codigo FROM suceso_iniciador WHERE etapa_id = ? AND subetapa_id = ? AND activo = ? ORDER BY codigo");
            qryEvents.addBindValue(stageId);
            qryEvents.addBindValue(substageId);
            qryEvents.addBindValue(1);  // activo = 1
            if (!qryEvents.exec()) {
                qDebug() << "    Query error:" << qryEvents.lastError().text();
            }
            
            qDebug() << "    Query for substage" << substageId << "executed, rows:" << qryEvents.size();
            
            while (qryEvents.next()) {
                int eventId = qryEvents.value(0).toInt();
                QString eventName = qryEvents.value(1).toString();
                QString eventCode = qryEvents.value(2).toString();
                
                qDebug() << "    Event:" << eventId << eventCode << eventName;
                
                // Check if this event already has this defense
                bool defenseExists = checkDefenseExists(eventId);
                
                // Create event item with checkbox
                QTreeWidgetItem *eventItem = new QTreeWidgetItem(substageItem);
                eventItem->setText(0, QString("%1 - %2").arg(eventCode).arg(eventName));
                eventItem->setFlags(eventItem->flags() | Qt::ItemIsUserCheckable);
                
                if (defenseExists) {
                    // Defense already exists - mark as checked, gray, and non-changeable
                    eventItem->setCheckState(0, Qt::Checked);
                    eventItem->setForeground(0, QBrush(Qt::gray));
                    eventItem->setFlags(eventItem->flags() & ~Qt::ItemIsEnabled);
                } else {
                    // Defense doesn't exist - normal checkbox
                    eventItem->setCheckState(0, Qt::Unchecked);
                }
                
                eventItem->setData(0, Qt::UserRole, eventId);
                eventItem->setData(0, Qt::UserRole + 1, "event");
                eventItem->setData(0, Qt::UserRole + 2, stageId);
                eventItem->setData(0, Qt::UserRole + 3, substageId);
                
                substageEventCount++;
                stageEventCount++;
            }
            
            qDebug() << "  Substage event count:" << substageEventCount;
            
            // Set substage flags based on whether it has events
            if (substageEventCount > 0) {
                substageItem->setFlags(substageItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsUserTristate);
                
                // Check if ALL events in this substage have the defense
                bool allEventsHaveDefense = true;
                for (int i = 0; i < substageItem->childCount(); ++i) {
                    QTreeWidgetItem *child = substageItem->child(i);
                    if (child->flags() & Qt::ItemIsEnabled) {
                        // At least one event doesn't have the defense
                        allEventsHaveDefense = false;
                        break;
                    }
                }
                
                if (allEventsHaveDefense) {
                    // All events have the defense - mark substage as checked, gray, and disabled
                    substageItem->setCheckState(0, Qt::Checked);
                    substageItem->setForeground(0, QBrush(Qt::gray));
                    substageItem->setFlags(substageItem->flags() & ~Qt::ItemIsEnabled);
                } else {
                    substageItem->setCheckState(0, Qt::Unchecked);
                }
            } else {
                // No events - gray it out, no checkbox
                substageItem->setForeground(0, QBrush(Qt::gray));
            }
        }
        
        // Query events directly under stage (without substage)
        QSqlQuery qryEventsNoSubstage;
        qryEventsNoSubstage.prepare("SELECT id, nombre, codigo FROM suceso_iniciador WHERE etapa_id = ? AND (subetapa_id IS NULL OR subetapa_id = 0) AND activo = ? ORDER BY codigo");
        qryEventsNoSubstage.addBindValue(stageId);
        qryEventsNoSubstage.addBindValue(1);  // activo = 1
        if (!qryEventsNoSubstage.exec()) {
            qDebug() << "  Direct events query error:" << qryEventsNoSubstage.lastError().text();
        }
        
        qDebug() << "  Query for direct events in stage" << stageId << "executed, rows:" << qryEventsNoSubstage.size();
        
        while (qryEventsNoSubstage.next()) {
            int eventId = qryEventsNoSubstage.value(0).toInt();
            QString eventName = qryEventsNoSubstage.value(1).toString();
            QString eventCode = qryEventsNoSubstage.value(2).toString();
            
            qDebug() << "  Direct event:" << eventId << eventCode << eventName;
            
            // Check if this event already has this defense
            bool defenseExists = checkDefenseExists(eventId);
            
            // Create event item with checkbox
            QTreeWidgetItem *eventItem = new QTreeWidgetItem(stageItem);
            eventItem->setText(0, QString("%1 - %2").arg(eventCode).arg(eventName));
            eventItem->setFlags(eventItem->flags() | Qt::ItemIsUserCheckable);
            
            if (defenseExists) {
                // Defense already exists - mark as checked, gray, and non-changeable
                eventItem->setCheckState(0, Qt::Checked);
                eventItem->setForeground(0, QBrush(Qt::gray));
                eventItem->setFlags(eventItem->flags() & ~Qt::ItemIsEnabled);
            } else {
                // Defense doesn't exist - normal checkbox
                eventItem->setCheckState(0, Qt::Unchecked);
            }
            
            eventItem->setData(0, Qt::UserRole, eventId);
            eventItem->setData(0, Qt::UserRole + 1, "event");
            eventItem->setData(0, Qt::UserRole + 2, stageId);
            
            stageEventCount++;
        }
        
        qDebug() << "Stage event count:" << stageEventCount;
        
        // Set stage flags based on whether it has events
        if (stageEventCount > 0) {
            stageItem->setFlags(stageItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsUserTristate);
            
            // Check if ALL events in this stage (including those in substages) have the defense
            bool allEventsHaveDefense = true;
            for (int i = 0; i < stageItem->childCount(); ++i) {
                QTreeWidgetItem *child = stageItem->child(i);
                QString childType = child->data(0, Qt::UserRole + 1).toString();
                
                if (childType == "substage") {
                    // Check if substage is enabled (if disabled, all its events have the defense)
                    if (child->flags() & Qt::ItemIsEnabled) {
                        // Substage is enabled, so not all its events have the defense
                        allEventsHaveDefense = false;
                        break;
                    }
                } else if (childType == "event") {
                    // Direct event under stage
                    if (child->flags() & Qt::ItemIsEnabled) {
                        // Event doesn't have the defense
                        allEventsHaveDefense = false;
                        break;
                    }
                }
            }
            
            if (allEventsHaveDefense) {
                // All events have the defense - mark stage as checked, gray, and disabled
                stageItem->setCheckState(0, Qt::Checked);
                stageItem->setForeground(0, QBrush(Qt::gray));
                stageItem->setFlags(stageItem->flags() & ~Qt::ItemIsEnabled);
            } else {
                stageItem->setCheckState(0, Qt::Unchecked);
            }
        } else {
            // No events - gray it out, no checkbox
            stageItem->setForeground(0, QBrush(Qt::gray));
        }
    }
    
    qDebug() << "Expanding all...";
    ui->treeEvents->expandAll();
    qDebug() << "Tree loaded with" << ui->treeEvents->topLevelItemCount() << "top level items";
}

bool DlgAddDefenseToEvents::checkDefenseExists(int eventId)
{
    QSqlQuery qry;
    qry.prepare("SELECT COUNT(*) FROM defensa WHERE suceso_iniciador_id = ? AND nombre = ? AND tipo = ?");
    qry.addBindValue(eventId);
    qry.addBindValue(defense.nombre);
    qry.addBindValue(static_cast<int>(defense.type));
    
    if (qry.exec() && qry.next()) {
        return qry.value(0).toInt() > 0;
    }
    
    return false;
}

void DlgAddDefenseToEvents::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) return;
    
    // Disconnect signal to avoid recursion
    disconnect(ui->treeEvents, &QTreeWidget::itemChanged, this, &DlgAddDefenseToEvents::onItemChanged);
    
    QString itemType = item->data(0, Qt::UserRole + 1).toString();
    
    // Update children check state if it's a parent item
    if (itemType == "stage" || itemType == "substage") {
        updateChildrenCheckState(item, item->checkState(0));
    }
    
    // Update parent check state
    updateParentCheckState(item);
    
    // Reconnect signal
    connect(ui->treeEvents, &QTreeWidget::itemChanged, this, &DlgAddDefenseToEvents::onItemChanged);
}

void DlgAddDefenseToEvents::updateChildrenCheckState(QTreeWidgetItem *item, Qt::CheckState state)
{
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);
        if (child->flags() & Qt::ItemIsUserCheckable) {
            child->setCheckState(0, state);
            // Recursively update children
            if (child->childCount() > 0) {
                updateChildrenCheckState(child, state);
            }
        }
    }
}

void DlgAddDefenseToEvents::updateParentCheckState(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if (!parent) return;
    
    int checkedCount = 0;
    int uncheckedCount = 0;
    int partiallyCheckedCount = 0;
    
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *child = parent->child(i);
        Qt::CheckState childState = child->checkState(0);
        
        if (childState == Qt::Checked) {
            checkedCount++;
        } else if (childState == Qt::Unchecked) {
            uncheckedCount++;
        } else if (childState == Qt::PartiallyChecked) {
            partiallyCheckedCount++;
        }
    }
    
    // Set parent state based on children
    if (checkedCount > 0 && uncheckedCount == 0 && partiallyCheckedCount == 0) {
        parent->setCheckState(0, Qt::Checked);
    } else if (uncheckedCount == parent->childCount()) {
        parent->setCheckState(0, Qt::Unchecked);
    } else {
        parent->setCheckState(0, Qt::PartiallyChecked);
    }
    
    // Recursively update parent's parent
    updateParentCheckState(parent);
}

QList<int> DlgAddDefenseToEvents::collectCheckedEventIds(QTreeWidgetItem *item) const
{
    QList<int> eventIds;
    
    QString itemType = item->data(0, Qt::UserRole + 1).toString();
    
    if (itemType == "event") {
        if (item->checkState(0) == Qt::Checked) {
            eventIds.append(item->data(0, Qt::UserRole).toInt());
        }
    } else {
        // Recursively collect from children
        for (int i = 0; i < item->childCount(); ++i) {
            eventIds.append(collectCheckedEventIds(item->child(i)));
        }
    }
    
    return eventIds;
}

QList<int> DlgAddDefenseToEvents::getSelectedEventIds() const
{
    QList<int> eventIds;
    
    for (int i = 0; i < ui->treeEvents->topLevelItemCount(); ++i) {
        eventIds.append(collectCheckedEventIds(ui->treeEvents->topLevelItem(i)));
    }
    
    return eventIds;
}

bool DlgAddDefenseToEvents::shouldApplyDefense() const
{
    return ui->rbApply->isChecked();
}

void DlgAddDefenseToEvents::on_btnCancel_clicked()
{
    reject();
}

void DlgAddDefenseToEvents::on_btnAdd_clicked()
{
    QList<int> selectedEventIds = getSelectedEventIds();
    
    if (selectedEventIds.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "No se ha seleccionado ningún suceso.");
        return;
    }
    
    accept();
}
