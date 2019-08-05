#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QWidget>

class ClockWidget : public QWidget
{
    Q_OBJECT

public:
    ClockWidget(QWidget *parent = 0);
    ~ClockWidget();

    QRectF textRectF(double radius, int pointSize, double angle);
    void paintEvent(QPaintEvent *event);

    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    QPoint z;
public slots:
    void on_clickAction();
    void menuEvent();

};

#endif // CLOCKWIDGET_H
