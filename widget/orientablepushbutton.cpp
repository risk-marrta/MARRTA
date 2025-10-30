#include "orientablepushbutton.h"

#include <QPainter>
#include <QStyleOptionButton>
#include <QDebug>
#include <QStylePainter>

OrientablePushButton::OrientablePushButton(QWidget *parent) : QPushButton (parent)
{ }

OrientablePushButton::OrientablePushButton(const QString &text, QWidget *parent): QPushButton (text, parent)
{ }

OrientablePushButton::OrientablePushButton(const QIcon &icon, const QString &text, QWidget *parent) : QPushButton (icon, text, parent)
{ }

OrientablePushButton::OrientablePushButton(const Orientation &orientation, const QIcon &icon, const QString &text, QWidget *parent) : QPushButton (icon, text, parent)
{ 
    mOrientation = orientation;
}

void OrientablePushButton::setOrientation(const OrientablePushButton::Orientation &orientation)
{
    mOrientation = orientation;
}

QSize OrientablePushButton::sizeHint() const
{
    QSize hint = QPushButton::sizeHint();
    if (mOrientation != Orientation::Horizontal) {
        hint.transpose();
    }

    return hint;
}

void OrientablePushButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStylePainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    if (mOrientation == Orientation::VerticalTopToBottom) {
        painter.rotate(90);
        painter.translate(0, -1 * width());
        option.rect = option.rect.transposed();
    } else if (mOrientation == Orientation::VerticalBottomToTop) {
        painter.rotate(-90);
        painter.translate(-1 * height(), 0);
        option.rect = option.rect.transposed();
    }

    painter.drawControl(QStyle::CE_PushButton, option);
}
