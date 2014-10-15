
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDesktopWidget"
#include "qdebug.h"
#include "string"
#include "QString"
#include <QKeyEvent>
#include <QtWin>
#include <vector>
#include <QPixmap>
#include <QScreen>
#include <map>
#include <Psapi.h>
#include <winbase.h>
#include "taskwindow.h"
#include <windows.h>
#include <QCursor>
#include <MyGlobalShortcut/MyGlobalShortCut.h>
struct MyWindow
{
    HWND hwnd;
    QString title;
    QPixmap pix;
    QPixmap icon;
};
QVector<MyWindow> win_vec;
std::vector<HWND>    hwnd_vec;
std::vector<HBITMAP> hbitmap_vec;
std::vector<QPixmap> qpixmap_vec;
QVector<TaskWindow*> task_vec;
BOOL CALLBACK MainWindow::EnumWindowsProc(HWND hwnd, LONG lParam)
{
    // 获取窗口标题
    TCHAR buffer[512];
    GetWindowText(hwnd, buffer,512);
    QString title = QString::fromWCharArray(buffer);
//    std::wstring bob(&buffer[0]);
//    std::string title(bob.begin(), bob.end());

    if (IsWindow(hwnd)
        &&  IsWindowVisible(hwnd)
        &&  GetParent(hwnd) == NULL
        &&  title != ""
        &&  title != "Program Manager")
    {
        //qDebug() << title << endl;
        // 获取所有窗口句柄
        hwnd_vec.push_back(hwnd);
        return true;
    }

    return TRUE;
}

bool MainWindow::ExtractIco(QString file_path, QPixmap &pic)
{
    const bool large = true;
    const QString sourceFile = file_path;

    const UINT iconCount = ExtractIconEx((wchar_t *)sourceFile.utf16(), -1, 0, 0, 0);
    if (!iconCount) {
        //std::cerr << sourceFile.toStdString() << " does not appear to contain icons.\n";
        return 1;
    }

    QScopedArrayPointer<HICON> icons(new HICON[iconCount]);
    const UINT extractedIconCount = large ?
        ExtractIconEx((wchar_t *)sourceFile.utf16(), 0, icons.data(), 0, iconCount) :
        ExtractIconEx((wchar_t *)sourceFile.utf16(), 0, 0, icons.data(), iconCount);

    if (!extractedIconCount) {
        //qErrnoWarning("Failed to extract icons from %s", qPrintable(sourceFile));
        return 1;
    }
    QPixmap pixmap = QtWin::fromHICON(icons[0]);
    if (pixmap.isNull())
    {
           // std::cerr << "Error converting icons.\n";
            return 1;
    }
    // 保存
    pic = pixmap;
    return 0;

}

void MainWindow::toclose()
{
    is_active = false;
    win_vec.clear();
    hwnd_vec.clear();
    hbitmap_vec.clear();
    qpixmap_vec.clear();

    for(int i = 0;i < task_vec.length();++i)
    {
        task_vec.at(i)->deleteLater();
        task_vec.at(i)->hide();
    }
    task_vec.clear();

    qDebug() << "toclose" << endl;
    this->hide();

}

void MainWindow::activate()
{
    if(is_active)
    {
        is_active = false;
        toclose();
        return;
    }
    is_active = true;

    // 1. 枚举所有窗口句柄,保存至 hwnd_vec
    EnumDesktopWindows(NULL,EnumWindowsProc,(LPARAM)0);
    qDebug() << "---------------------" << endl;
    for(std::vector<HWND>::iterator i = hwnd_vec.begin(); i != hwnd_vec.end();++i)
        if(*i == (HWND)this->winId())
        {
            hwnd_vec.erase(i);
            break;
        }
    /* 2. 对每个句柄所在窗口截图，保存至 qpixmap_vec */
//    for(std::vector<HWND>::iterator i = hwnd_vec.begin(); i != hwnd_vec.end();++i)
//        GrabWindowSnap(*i);
     for(std::vector<HWND>::iterator i = hwnd_vec.begin(); i != hwnd_vec.end();++i)
     {
             HWND hwnd = *i;
             MyWindow win;
             win.hwnd  = hwnd;
             TCHAR buffer[512];
             GetWindowText(hwnd, buffer,512);
             win.title = QString::fromWCharArray(buffer);
            // win.pix = QtWin::fromHBITMAP(hbmp,QtWin::HBitmapAlpha);
             win_vec.push_back(win);
     }


//    // 测试，将qpixmap保存
//    for(QVector<MyWindow>::iterator i = win_vec.begin(); i != win_vec.end();++i)
//    {
//        i->pix.save(i->title.append(".bmp"));
//    }
//    for(QVector<MyWindow>::iterator i = win_vec.begin(); i != win_vec.end();++i)
//    {
//        static int j = 0;
//        TaskWindow* task = new TaskWindow(this,i->hwnd,i->pix,i->title,QSize(400,200));
//        task->setGeometry(450 * j + 100,300,400,200);
//        j++;
//        task->show();
//    }

    // 获取进程文件地址
    //  http://blog.csdn.net/carl2380/article/details/5212928
    for (QVector<MyWindow>::iterator i = win_vec.begin(); i != win_vec.end();++i)
        {
            DWORD dwProcessId;
            GetWindowThreadProcessId(i->hwnd, &dwProcessId);
            HANDLE h_Process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
            WCHAR path[MAX_PATH] = {0};
            DWORD size = MAX_PATH;
            /* define _WIN32_WINNT as 0x0600 or later. */
            if(QueryFullProcessImageName(h_Process, 0, path, &size))
            {
                std::wstring bob(&path[0]);
                std::string title(bob.begin(), bob.end());
                //std::string title(path);
                //qDebug() << title.c_str() << endl;
                QString file_path(title.c_str());
                QPixmap pix;
                //qDebug() << "标题: " << i->title << endl << "路径: " << title.c_str() << endl;
                if(!ExtractIco(file_path,pix))
                {
                    //qDebug() << "图标: yes" << endl;
                    i->icon = pix;
                }
                else
                {
                    //qDebug() << "图标： no" <<  endl;

                   i->icon = QPixmap();
                }
            }
            else
            {

               // qDebug() << "标题 : " << i->title << " 路径： " << "no" << endl << "error:" << GetLastError() << endl;
            }



        }
    int taskcount = 0;
    for(QVector<MyWindow>::iterator i = win_vec.begin(); i != win_vec.end();++i)
        taskcount++;
    int j = 0;
    QPoint pos = QCursor::pos();
    for(QVector<MyWindow>::iterator i = win_vec.begin(); i != win_vec.end();++i)
    {


        TaskWindow* task = new TaskWindow(this,i->hwnd,i->icon,i->title,QSize(64,64));
       // task->setGeometry(m_desk.width() * 0.4 + (j - taskcount/2) * 50 ,m_desk.height() * 0.3 ,64,64);
        task->setGeometry(pos.x() - 30 + (j - taskcount/2) * 50 ,pos.y() - 60 ,64,64);
        j++;
        connect(task,SIGNAL(clicked()),this,SLOT(toclose()));
        task_vec.push_back(task);
        //task->show();
    }


    this ->setWindowFlags(Qt::FramelessWindowHint
                          |Qt::Tool // 不在任务栏显示
                          |Qt::WindowStaysOnTopHint
                          );

    // 顺序显示窗口和按钮
     this->show();
    for(int i = 0;i < task_vec.length();++i)
    {
        task_vec.at(i)->show();

    }



}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),is_active(false)
{

    Init();         // 初始化背景透明
    //activate();     // 热键触发
    this->hide();

    MyGlobalShortCut* shortcut = new MyGlobalShortCut("Alt+1",this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(activate()));


}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
        this->close();
    QMainWindow::keyPressEvent(e);
}

void MainWindow::Init()
{

    // 设置窗体大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->screenGeometry();
    m_desk = deskRect;
    this ->resize(deskRect.width(),deskRect.height());

    // 去除标题栏
    this ->setWindowFlags(Qt::FramelessWindowHint
                          |Qt::Tool // 不在任务栏显示
                          );
    // 设置窗体透明
    this ->setAttribute(Qt::WA_TranslucentBackground);



    // 设置背景
//    QScreen *screen = QGuiApplication::primaryScreen();
//    QPixmap pixmap = screen->grabWindow(0);
//    QPalette   palette;
//    palette.setBrush(this->backgroundRole(),QBrush(pixmap));
//    this->setPalette(palette);

}

void MainWindow::GrabWindowSnap(HWND hwnd)
{
    RECT rc;
    /*
    *  1. 对于已经最小化的窗口
    *     关闭窗口动画
    *     设置窗口透明
    */

    WINDOWPLACEMENT wp = { 0 };
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &wp);

    ANIMATIONINFO ai = { 0 };
    bool restoreAnimated = false;

    if (wp.showCmd == SW_SHOWMINIMIZED)
    {
        // 获取效果状态
        ai.cbSize = sizeof(ANIMATIONINFO);
        SystemParametersInfo(SPI_GETANIMATION, sizeof(ANIMATIONINFO), &ai, 0);

        // 如果动画效果开启，则关闭
        if (ai.iMinAnimate != 0)
        {
            ai.iMinAnimate = 0;
            SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0);
            restoreAnimated = true;
        }
        // 设置为透明

        long winLong = GetWindowLong(hwnd, GWL_EXSTYLE);
        SetWindowLong(hwnd, GWL_EXSTYLE, winLong | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hwnd, 0, 1, LWA_ALPHA);

        // 显示窗口
        ShowWindow(hwnd, SW_SHOWNORMAL);
        //ShowWindow(hwnd, SW_MAXIMIZE);

    }

    /*
    * 2. 截屏操作
    */

    GetClientRect(hwnd, &rc);

    //创建
    HDC hdcScreen = GetDC(hwnd);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdc, hbmp);

    //复制
    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcScreen, 0, 0, SRCCOPY);
    //PrintWindow(hwnd, hdc, PW_CLIENTONLY);
    //PW_CLIENTONLY：Only the client area of the window is copied to hdcBlt.
    //By default, the entire window is copied.
    //PW_CLIENTONLY表示仅仅拷贝窗口的客户区域，而默认情况下，执行printwindow会拷贝整个窗口

    //添加到容器中
    do
    {
        MyWindow win;
        win.hwnd  = hwnd;
        TCHAR buffer[512];
        GetWindowText(hwnd, buffer,512);
        win.title = QString::fromWCharArray(buffer);
        win.pix = QtWin::fromHBITMAP(hbmp,QtWin::HBitmapAlpha);
        win_vec.push_back(win);
    }while(0);



    // 添加到剪贴板
   // OpenClipboard(NULL);
   // EmptyClipboard();
   // SetClipboardData(CF_BITMAP, hbmp);
    //CloseClipboard();

    //释放
    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);
    //qDebug()<< "成功把记事本窗口复制到粘贴板,请粘贴到Windows画图工具" << endl;

    /*
    * 3. 逆转1的操作
    */

    // 恢复动画效果
    if (wp.showCmd == SW_SHOWMINIMIZED)
    {
        SetWindowPlacement(hwnd, &wp);

        // optionally remove alpha using SetLayeredWindowAttributes()...

        if (restoreAnimated)
        {
            ai.iMinAnimate = 1;
            SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0);
        }
        // 恢复透明
        long winLong = GetWindowLong(hwnd, GWL_EXSTYLE);
        SetWindowLong(hwnd, GWL_EXSTYLE, winLong^WS_EX_LAYERED);
    }

}

