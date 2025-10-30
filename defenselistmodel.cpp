#include "defenselistmodel.h"

#include <QColor>
#include <QDebug>

DefenseListModel::DefenseListModel(QObject *parent): QStringListModel (parent) {}

DefenseListModel::DefenseListModel(const QStringList &strings, QObject *parent):
    QStringListModel(strings, parent){

}

Qt::ItemFlags DefenseListModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
    if (index.isValid()){
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}

bool DefenseListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || role != Qt::CheckStateRole)
        return false;

    if(value == Qt::Checked)
        checkedItems.insert(index);
    else
        checkedItems.remove(index);

    emit dataChanged(index, index);
    return true;
}

QVariant DefenseListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::CheckStateRole)
        return checkedItems.contains(index) ?
                    Qt::Checked : Qt::Unchecked;

    /*else if(role == Qt::BackgroundColorRole)
        return checkedItems.contains(index) ?
                    QColor("#ffffb2") : QColor("#ffffff");*/

    return QStringListModel::data(index, role);
}

void DefenseListModel::setChecked(const QStringList &strings)
{
    checkedItems.clear();

    for (auto s : strings) {
        auto f = match(index(0, 0), Qt::DisplayRole, s);
        if (f.count() > 0) {
            setData(f[0], Qt::Checked, Qt::CheckStateRole);
        }
    }
}
