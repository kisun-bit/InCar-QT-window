#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <QWidget>
#include <sys/fcntl.h>
#include "stdio.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <QProcess>

namespace Ui {
class LedControl;
}


class LedControl : public QWidget
{
    Q_OBJECT

public:
    explicit LedControl(QWidget *parent=nullptr);
    ~LedControl();

private slots:
    void on_pushButton_clicked();
//    void open_led();
//    void close_led();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();


private:
    Ui::LedControl *ui;
    void open_led();
    void close_led();
//    QProcess *led_process;
    int fd;
};

#endif // LEDCONTROL_H
