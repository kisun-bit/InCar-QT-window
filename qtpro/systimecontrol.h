#ifndef SYSTIMECONTROL_H
#define SYSTIMECONTROL_H

#include <QWidget>

namespace Ui {
class SysTimeControl;
}

class SysTimeControl : public QWidget
{
    Q_OBJECT

public:
    explicit SysTimeControl(QWidget *parent = nullptr);
    ~SysTimeControl();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SysTimeControl *ui;
};

#endif // SYSTIMECONTROL_H
