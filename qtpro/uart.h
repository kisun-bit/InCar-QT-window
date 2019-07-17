#ifndef UART_H
#define UART_H

#include <QWidget>
#include <QtSerialPort>


namespace Ui {
class Uart;
}

class Uart : public QWidget
{
    Q_OBJECT

public:
    explicit Uart(QWidget *parent = nullptr);
    ~Uart();

private slots:
    void on_closeBtn_clicked();

    void on_openPortBtn_clicked();

    void on_sendBtn_clicked();

private:
    Ui::Uart *ui;
    void init();
    QSerialPort *serialport;
    QByteArray QString2Hex(QString data);
    char ConvertHexChar(char ch);
};

#endif // UART_H
