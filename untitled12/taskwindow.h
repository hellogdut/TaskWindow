#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QPushButton>
#include <windows.h>
#include <QSize>
#include <QVector>
class TaskWindow : public QPushButton
{
    Q_OBJECT
public:
    explicit TaskWindow(QWidget *parent = 0);
    TaskWindow(QWidget *parent,HWND hwnd,QPixmap pix,QString title,QSize size);

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);
    void GenerateGif();
    void xuanwo(double para);
    void timerEvent(QTimerEvent *e);

signals:

private:
    HWND    m_hwnd;
    QPixmap m_pix;
    QString m_title;
    QSize   m_size;
    QVector<QImage> m_vec;
    QPoint  m_pos;
    int     m_timer_id;
    int     index;
    int     has_pos;

    static const int     m_pixCount = 30;



public slots:

};

#endif // TASKWINDOW_H
