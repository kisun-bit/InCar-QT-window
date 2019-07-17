#include "ledcontrol.h"
#include "ui_ledcontrol.h"
#include <QMessageBox>
#include <QDebug>


enum LED { ONE, TWO, THREE, FOUR }; //灯的编号

enum LED_CMD { OFF, ON }; // 控制灯的开关   OFF:0   ON:1


LED_CMD status;


LedControl::LedControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedControl)
{
    ui->setupUi(this);
//    led_process = new  QProcess(this);
    status = LED_CMD::OFF;
}
//

LedControl::~LedControl()
{
    delete ui;
}

void LedControl::on_pushButton_clicked()
{
    this->close();
}

void LedControl::on_radioButton_clicked()
{
    //    void open_led();
    open_led();
}

void LedControl::on_radioButton_2_clicked()
{
    //    void close_led();
    close_led();
}


void LedControl::open_led(){
    fd = ::open("/dev/leds", 0);
    if(fd < 0 && status==LED_CMD::OFF){
        QMessageBox::warning(nullptr, tr("LED_IO_Controller"), tr("led 设备打开失败"));

    }else {
        QMessageBox::about(nullptr, tr("LED_IO_Controller"), tr("注意查看led灯的变化"));
        ::ioctl(fd, LED_CMD::ON, LED::ONE);
        ::ioctl(fd, LED_CMD::ON, LED::TWO);
        ::ioctl(fd, LED_CMD::ON, LED::THREE);
        ::ioctl(fd, LED_CMD::ON, LED::FOUR);
        status = LED_CMD::ON;
//        ::close(fd);
    }
}

void LedControl::close_led(){
    if(status==LED_CMD::ON){
        ::ioctl(fd, LED_CMD::OFF, LED::ONE);
        ::ioctl(fd, LED_CMD::OFF, LED::TWO);
        ::ioctl(fd, LED_CMD::OFF, LED::THREE);
        ::ioctl(fd, LED_CMD::OFF, LED::FOUR);
    }
}

