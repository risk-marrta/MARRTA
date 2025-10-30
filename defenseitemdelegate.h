#ifndef DEFENSEITEMDELEGATE_H
#define DEFENSEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>

class DefenseItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DefenseItemDelegate(QObject *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QPair<QString, QString> splitDefenseDisplayText(const QString &fullText) const;
};

#endif // DEFENSEITEMDELEGATE_H
