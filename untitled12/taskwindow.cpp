#include "taskwindow.h"
#include "QPropertyAnimation"
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <math.h>
#include "QDesktopWidget"
#include <QApplication>
template<typename T> T sqr(T x){return x*x;}

TaskWindow::TaskWindow(QWidget *parent) :
    QPushButton(parent)
{
}

TaskWindow::TaskWindow(QWidget *parent, HWND hwnd, QPixmap pix, QString title, QSize size)
    :QPushButton(parent),m_hwnd(hwnd),m_pix(pix),m_title(title),m_size(size),m_timer_id(0),has_pos(0)

{



    this->setStyleSheet("background-color:transparent");

    this->setIconSize(m_size);
    this->resize(size);
    GenerateGif();

//    m_pix = m_pix.scaled(32,32,Qt::KeepAspectRatio);
//    this->setIcon(QIcon(m_pix));




//    this->setIcon(QIcon(m_pix));
//    this ->setWindowFlags(Qt::FramelessWindowHint);
//    QPalette   palette;
//    palette.setBrush(this->backgroundRole(),QBrush(pix));
    //    this->setPalette(palette);
}

void TaskWindow::enterEvent(QEvent *e)
{

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->screenGeometry();

    RECT rc;

    /* 1. 类似 alt_tab 效果 */
//    ShowWindow(m_hwnd,SW_SHOWNA);
//    GetWindowRect(m_hwnd,&rc);
//    SetWindowPos(m_hwnd,HWND_TOPMOST,rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,SWP_NOMOVE);

    /* 2. 最大化窗口效果 */
    ShowWindow(m_hwnd,SW_MAXIMIZE);
    GetWindowRect(m_hwnd,&rc);
    SetWindowPos(m_hwnd,HWND_TOP,rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,SWP_NOMOVE);
    //SetWindowPos(m_hwnd,HWND_BOTTOM,rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,SWP_NOMOVE);

    QPushButton::enterEvent(e);
    emit clicked();



}

void TaskWindow::leaveEvent(QEvent *e)
{

}

void TaskWindow::mousePressEvent(QMouseEvent *e)
{
    //QPushButton::mousePressEvent(e);


    emit clicked();
}

void TaskWindow::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);
}

void TaskWindow::GenerateGif()
{
    for(int i = 0;i < m_pixCount;++i)
        xuanwo(i);
    index = m_vec.length() - 1;
    m_timer_id = startTimer(5);

}

void TaskWindow::xuanwo(double para)
{
    double Pi=3.14;
    QImage src = QPixmap(m_pix).toImage();
    int width = src.width();
    int height = src.height();
   // ui->label->resize(width,height);
    QImage img = src;
    QPoint center(width/2,height/2);
    if(para == 0)
    {
        m_vec.push_back(img);
        //img.save(QString::number(para).append(m_title).append(".bmp"));
        return ;
    }

    for(int y = 0; y < height; ++y)
    {

        for(int x = 0; x < width ; ++x)
        {
            int R = sqrt( sqr(x - center.x()) + (sqr(y - center.y())));
            double angle = atan2((double)(y - center.y()),(double)(x - center.x()));
            double delta = para * Pi / sqrt(R + 1);

            int newX = R * cos(angle+delta) + center.x();
            int newY = R * sin(angle+delta) + center.y();

            if(newX<0) newX=0;
            if(newX>width-1) newX=width-1;
            if(newY<0) newY=0;
            if(newY>height-1) newY=height-1;

           img.setPixel(x,y,src.pixel(newX,newY));
        }
    }
   // img.save(QString::number(para).append(m_title).append(".bmp"));
    m_vec.push_back(img);
}

void TaskWindow::timerEvent(QTimerEvent *e)
{
    if(index < 0)
    {
        killTimer(m_timer_id);
        return;
    }
    //qDebug() << m_title << " : " <<  index;
    this->setIcon(QIcon(QPixmap::fromImage(m_vec.at(index)).scaled(32,32,Qt::KeepAspectRatio)));
    --index;
}






