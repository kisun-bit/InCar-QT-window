#include "mainmeun.h"
#include "ui_mainmeun.h"
#include <QtWidgets>

MainWindow *MainMeun::game=nullptr;
VideoWidget *MainMeun::video=nullptr;
QTimer *MainMeun::sys_timer=nullptr;
LedControl *MainMeun::led=nullptr;
music *MainMeun::music_w=nullptr;
Uart *MainMeun::uart_widget=nullptr;
SysTimeControl *MainMeun::time_widget = nullptr;
Usb *MainMeun::usb_widget = nullptr;

MainMeun::MainMeun(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMeun)
{
    ui->setupUi(this);
    init();
}

void MainMeun::init(){
    sys_timer = new QTimer(this);
    connect(sys_timer, SIGNAL(timeout()), this, SLOT(flush_time()));
    sys_timer->start(500);
}

void MainMeun::flush_time(){
    ui->lcdNumber->display(QTime::currentTime().toString("hh:mm:ss"));
}

MainMeun::~MainMeun()
{
    delete ui;
}

void MainMeun::on_pushButton_6_clicked()
{
    game = new MainWindow();
    game->show();
}

void MainMeun::on_pushButton_3_clicked()
{
    video = new VideoWidget();
    video->show();
}

void MainMeun::on_pushButton_12_clicked()
{
   led = new LedControl();
   led->show();
}



void MainMeun::on_pushButton_clicked()
{
    music_w = new music();
    music_w->show();
}

void MainMeun::on_pushButton_2_clicked()
{
    time_widget = new SysTimeControl();
    time_widget->show();
}

void MainMeun::on_pushButton_5_clicked()
{
    uart_widget = new Uart();
    uart_widget->show();
}

void MainMeun::on_pushButton_10_clicked()
{
    usb_widget = new Usb();
    usb_widget->show();
}
