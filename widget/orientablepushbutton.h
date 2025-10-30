#ifndef ORIENTABLEPUSHBUTTON_H
#define ORIENTABLEPUSHBUTTON_H

#include <QObject>
#include <QPushButton>

class OrientablePushButton : public QPushButton
{
    Q_OBJECT
public:
    enum class Orientation {
        Horizontal,
        VerticalTopToBottom,
        VerticalBottomToTop
    };

    explicit OrientablePushButton(QWidget *parent = nullptr);
    explicit OrientablePushButton(const QString &text, QWidget *parent = nullptr);
    explicit OrientablePushButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    explicit OrientablePushButton(const OrientablePushButton::Orientation &orientation, const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    QSize sizeHint() const;

    OrientablePushButton::Orientation orientation() const;
    void setOrientation(const OrientablePushButton::Orientation &orientation);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Orientation mOrientation = Orientation::Horizontal;
};


#endif // ORIENTABLEPUSHBUTTON_H
