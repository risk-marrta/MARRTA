#ifndef DEFENSELISTMODEL_H
#define DEFENSELISTMODEL_H

#include <QStringListModel>
#include <QSet>

class DefenseListModel : public QStringListModel
{
public:
    DefenseListModel(QObject* parent = 0);
    DefenseListModel(const QStringList & strings, QObject* parent = 0);
    Qt::ItemFlags flags (const QModelIndex& index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setChecked(const QStringList & strings);
    const QStringList & getChecked();

private:
    QSet<QPersistentModelIndex> checkedItems;
};

#endif // DEFENSELISTMODEL_H
