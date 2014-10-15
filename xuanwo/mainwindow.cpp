#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPoint>
#include <QImage>
#include <QtMath>
#include <math.h>
#include <QDebug>
double Para=0;
using namespace std;
QVector<QImage> pix_vec;
const int PIX_COUNT = 60;

template<typename T> T sqr(T x){return x*x;}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),id(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap(":/new/prefix1/1.bmp"));
    GenerateGif();


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::xuanwo(double para)
{

    double Pi=3.14;
    QImage src = QPixmap(":/new/prefix1/1.bmp").toImage();//ui->label->pixmap()->toImage();
    int width = src.width();
    int height = src.height();    
   // ui->label->resize(width,height);
    QImage img = src;
    QPoint center(width/2,height/2);
    if(para == 0)
    {
        pix_vec.push_back(img);
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
    /* 翻转 */
//    for(int x = 0;x <= width / 2;++x)
//    {
//        for(int y = 0; y < height;++y)
//        {
//           int red,green,blue;
//           red = qRed(src.pixel(x,y));
//           green = qGreen(src.pixel(x,y));
//           blue = qBlue(src.pixel(x,y));
//           QRgb origin = src.pixel(x,y);
//           QRgb tar    = src.pixel(width - x - 1,y);
//           img.setPixel(x, y,tar);
//           img.setPixel(width - x - 1,y,origin);
//        }
//    }
    img.save(QString::number(para).append(".bmp"));
    pix_vec.push_back(img);
    //ui->label->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::GenerateGif()
{
    for(int i = 0;i < PIX_COUNT;++i)
        xuanwo(i);

    id = startTimer(10);

}

void MainWindow::timerEvent(QTimerEvent *)
{

    static int i = pix_vec.length() - 1;
    qDebug() << i << endl;
    if(i < 0)
    {
        killTimer(id);
        return;
    }
    ui->label->setPixmap(QPixmap::fromImage(pix_vec.at(i)));
    --i;

}

void MainWindow::on_verticalSlider_actionTriggered(int action)
{
}

void MainWindow::setpix(QImage &pix)
{
    ui->label->setPixmap(QPixmap::fromImage(pix));
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    qDebug() << value << endl;
    xuanwo(value);
}
