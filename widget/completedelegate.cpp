#include "completedelegate.h"

#include <QStandardItem>
#include <QRect>
#include <QPainterPath>
#include <QApplication>

#include <QtDebug>

#include "styles.h"

int CompleteDelegate::padding = 5;
int CompleteDelegate::flagWidth = 30;
int CompleteDelegate::doublePadding = CompleteDelegate::padding << 1;
int CompleteDelegate::padding2AndFlag = CompleteDelegate::doublePadding + CompleteDelegate::flagWidth;
int CompleteDelegate::padding8AndFlag2 = (CompleteDelegate::padding << 3) + (CompleteDelegate::flagWidth << 1);
int CompleteDelegate::flagMinusPadding2 = CompleteDelegate::flagWidth - CompleteDelegate::doublePadding;

void CompleteDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    painter->save();

    QStyleOptionViewItem opt = option;

    if (option.state & QStyle::State_Selected)
         painter->fillRect(option.rect, option.palette.brush(QPalette::Highlight));

    QFont font = QApplication::font();
    QFontMetrics itemFm(font);
    auto text = index.data(Qt::DisplayRole).toString();
    int riesgo = index.data(0x1001).toInt();
    QRect itemRect =
            itemFm.boundingRect(option.rect.left() + flagWidth + padding, option.rect.top() + padding, option.rect.width() - padding8AndFlag2, 0, Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap, text);

    // Always use normal text color - let only background show selection
    // This ensures text is readable with light selection backgrounds
    painter->setPen(option.palette.color(QPalette::Text));
    painter->drawText(option.rect.x() + flagWidth, option.rect.y() + padding, option.rect.width() - padding2AndFlag, itemRect.height() + doublePadding, Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap, text);

    QPainterPath path;
    path.addRect(QRectF(option.rect.x() + padding, itemRect.y(), flagMinusPadding2, itemFm.height()));
    QPen pen(Qt::black, 1);
    painter->setPen(pen);
    if (riesgo >= 0) {
        switch (riesgo) {
            case 0:
                painter->fillPath(path, Styles::WhiteBackgroundColor);
                break;
            case 1:
                painter->fillPath(path, Styles::GreenBackgroundColor);
                break;
            case 2:
                painter->fillPath(path, Styles::YellowBackgroundColor);
                break;
            case 3:
                painter->fillPath(path, Styles::RedBackgroundColor);
                break;

        }
    } else {
        painter->setBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));
    }
    painter->drawPath(path);

    painter->restore();
}

QSize CompleteDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QSize();
    }

    QFont font = QApplication::font();
    QFontMetrics itemFm(font);
    auto text = index.data(Qt::DisplayRole).toString();
    QRect itemRect =
            itemFm.boundingRect(0, 0, option.rect.width() - padding8AndFlag2, 0, Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap, text);

    QSize result(option.rect.width(), itemRect.height() + doublePadding);
    return result;
}
