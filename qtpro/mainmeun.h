#ifndef MAINMEUN_H
#define MAINMEUN_H

#include <QWidget>
#include "mainwindow.h"
#include "videowidget.h"
#include <QTimer>
#include <QTime>
#include "ledcontrol.h"
#include "music.h"
#include "uart.h"
#include "systimecontrol.h"
#include "usb.h"

namespace Ui {
class MainMeun;
}

class MainMeun : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainMeun(QWidget *parent=0);
    ~MainMeun();
    static MainWindow *game;
    static VideoWidget *video;
    static QTimer *sys_timer;
    static LedControl *led;
    static music *music_w;
    static Uart *uart_widget;
    static SysTimeControl *time_widget;
    static Usb *usb_widget;
    
private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

    void flush_time();
    void init();

    void on_pushButton_12_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainMeun *ui;
};

#endif // MAINMEUN_H
