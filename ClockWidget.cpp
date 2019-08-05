#include "ClockWidget.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include<QtMath>
#include<QMouseEvent>
#include<QMessageBox>
#include<QApplication>
#include<QRect>
#include<QDesktopWidget>
#include<QFile>
#include<QTextStream>
#include <QFileDialog>
#include<QDebug>
#include <QMenu>
#include "ClockWidget.h"
#include"windows.h"
#define M_PI  3.14159
ClockWidget::ClockWidget(QWidget *parent)
    : QWidget(parent)
{
//    this->setWindowFlag(Qt::FramelessWindowHint);
//    this->setWindowFlag(Qt::SubWindow);

//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    QRect screenRect = desktopWidget->geometry();
//    this->resize(screenRect.width(),screenRect.height());

    this->setAttribute(Qt::WA_TranslucentBackground, true);//透明
    this->setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint);
    this->setWindowFlag(Qt::FramelessWindowHint);//无默认框
    // 构造定时器，设置超时为 1 秒
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

ClockWidget::~ClockWidget()
{

}
QRectF ClockWidget::textRectF(double radius, int pointSize, double angle)
{
    QRectF rectF;
    rectF.setX(radius*qCos(angle*M_PI/180.0) - pointSize*2);
    rectF.setY(radius*qSin(angle*M_PI/180.0) - pointSize/2.0);
    rectF.setWidth(pointSize*4);
    rectF.setHeight(pointSize);
    return rectF;
}
//注意：这里，textRectF() 主要用于获取小时对应的文本区域。
void ClockWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 时针、分针、秒针位置 - 多边形
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -30)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -65)
    };

    static const QPoint secondHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -80)
    };

    // 时针、分针、秒针颜色
    QColor hourColor(255, 100, 10, 200);
    QColor minuteColor(0, 127, 127, 150);
    QColor secondColor(255, 0, 0);

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    // 平移坐标系原点至中心点
    painter.translate(width() / 2, height() / 2);
    // 缩放
    painter.scale(side / 200.0, side / 200.0);

    // 绘制时针
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    painter.save();
    // 每圈360° = 12h 即：旋转角度 = 小时数 * 30°
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    painter.setPen(hourColor);

    // 绘制小时线 （360度 / 12 = 30度）
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    int radius = 100;
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    int pointSize = font.pointSize();

    // 绘制小时文本
    int nHour = 0;
    for (int i = 0; i < 12; ++i) {
        nHour = i + 3;
        if (nHour > 12)
            nHour -= 12;
        painter.drawText(textRectF(radius*0.8, pointSize, i * 30), Qt::AlignCenter, QString::number(nHour));
    }

    // 绘制分针
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);

    painter.save();
    // 每圈360° = 60m 即：旋转角度 = 分钟数 * 6°
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setPen(minuteColor);

    // 绘制分钟线 （360度 / 60 = 6度）
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }

    // 绘制秒针
    painter.setPen(Qt::NoPen);
    painter.setBrush(secondColor);

    painter.save();
    // 每圈360° = 60s 即：旋转角度 = 秒数 * 6°
    painter.rotate(6.0 * time.second());
    painter.drawConvexPolygon(secondHand, 3);
    painter.restore();
}
void ClockWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    QPoint y=event->globalPos();
    QPoint x=y-this->z;
    this->move(x);

}
void ClockWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    QPoint y=event->globalPos();
    QPoint x=this->geometry().topLeft();
    this->z=y-x;
}
void ClockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    this->z=QPoint();
//    QPoint zx=event->globalPos();
//    qDebug()<<zx.x()<<zx.y();
    if (event->button() == Qt::RightButton)//判断事件为鼠标右键
    {
        on_clickAction();
    }
}

void ClockWidget::keyReleaseEvent(QKeyEvent *event)
{
    //    if(event->key()==Qt:_)
}

void ClockWidget::on_clickAction()
{
        QMenu *pMenu = new QMenu(this);


        QAction *pEditTask = new QAction(tr("设置"), this);
        QAction *pTaskClose = new QAction(tr("关闭"), this);

        pEditTask->setData(1);
        pTaskClose->setData(2);


        pMenu->addAction(pEditTask);
        pMenu->addAction(pTaskClose);

        connect(pEditTask, SIGNAL(triggered()), this, SLOT(menuEvent()));
        connect(pTaskClose, SIGNAL(triggered()), this, SLOT(menuEvent()));

        pMenu->exec(cursor().pos());
}

void ClockWidget::menuEvent()
{
    QAction *pEven = qobject_cast<QAction *>(this->sender()); //this->sender()就是发信号者 QAction

    //获取发送信息类型 1:新建任务 2:设置任务 3:删除任务 4:改名工具 5:设置工具 6:删除工具
    int iType = pEven->data().toInt();

    switch (iType)
    {
    case 1:
        QMessageBox::about(this, "tip", pEven->text());
        break;
    case 2:
        QMessageBox::about(this, "tip", pEven->text());
        this->close();
        break;
    default:
        break;
    }
}
