#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qt_windows.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // win32
    void keyPressEvent(QKeyEvent *);
    // 初始化界面
    void   Init();
    // 获取窗口预览
    void   GrabWindowSnap(HWND hwnd);
    // 获取窗口句柄
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LONG lParam);
    // 获取程序图标
    bool ExtractIco(QString file_path,QPixmap& pic);
    //　智能隐藏
    void timerEvent(QTimerEvent *);

public slots:
    void toclose();
    void activate();
private:
    QRect m_desk;
    QRect m_taskrect;
    bool    is_active;

};


#endif // MAINWINDOW_H
