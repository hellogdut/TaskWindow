#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void xuanwo(double);
    void GenerateGif();
    void timerEvent(QTimerEvent *);
private slots:
    void on_verticalSlider_actionTriggered(int action);
    void setpix(QImage& pix);
    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    int id;
};

#endif // MAINWINDOW_H
