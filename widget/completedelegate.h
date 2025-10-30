#ifndef COMPLETEDELEGATE_H
#define COMPLETEDELEGATE_H

#include <QStyledItemDelegate>

#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>

class CompleteDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    static int padding;
    static int flagWidth;
    static int doublePadding;
    static int padding2AndFlag;
    static int padding8AndFlag2;
    static int flagMinusPadding2;
};

#endif // COMPLETEDELEGATE_H
