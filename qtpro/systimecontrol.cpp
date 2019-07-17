#include "systimecontrol.h"
#include "ui_systimecontrol.h"
#include <QDateTime>
#include <QMessageBox>

SysTimeControl::SysTimeControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysTimeControl)
{
    ui->setupUi(this);
}

SysTimeControl::~SysTimeControl()
{
    delete ui;
}

void SysTimeControl::on_pushButton_2_clicked()
{
    this->close();
}


void SysTimeControl::on_pushButton_clicked()
{
    // modify the system time
    QDateTime dt = QDateTime::currentDateTime();
    dt.setTime(ui->dateTimeEdit->time());
    time_t tt = (time_t)dt.toTime_t();
    int r = stime(&tt);
    if (r)
    {
        if (errno == EPERM)QMessageBox::warning(this, "Error", "没有权限");
    }
}
