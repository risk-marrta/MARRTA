#ifndef DLGADDDEFENSETOEVENTS_H
#define DLGADDDEFENSETOEVENTS_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "defensedefinition.h"

namespace Ui {
class DlgAddDefenseToEvents;
}

class DlgAddDefenseToEvents : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddDefenseToEvents(const DefenseDefinition &defense, QWidget *parent = nullptr);
    ~DlgAddDefenseToEvents();

    QList<int> getSelectedEventIds() const;
    bool shouldApplyDefense() const;

private slots:
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();
    void onItemChanged(QTreeWidgetItem *item, int column);

private:
    Ui::DlgAddDefenseToEvents *ui;
    DefenseDefinition defense;
    
    void loadEventTree();
    bool checkDefenseExists(int eventId);
    void updateChildrenCheckState(QTreeWidgetItem *item, Qt::CheckState state);
    void updateParentCheckState(QTreeWidgetItem *item);
    QList<int> collectCheckedEventIds(QTreeWidgetItem *item) const;
};

#endif // DLGADDDEFENSETOEVENTS_H
