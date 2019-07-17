#include "uart.h"
#include "ui_uart.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QList>
#include <QMessageBox>

Uart::Uart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Uart)
{
    ui->setupUi(this);
    init();


}

Uart::~Uart()
{
    delete ui;
}

void Uart::on_closeBtn_clicked()
{
    this->close();
}

void Uart::init(){


    ui->openPortBtn->setEnabled(false);
    ui->closePortBtn->setEnabled(false);
    ui->sendBtn->setEnabled(false);
    ui->resvBtn->setEnabled(false);
    ui->serials_box->addItem("选择串口");
    ui->baudrate->addItem("设置波特率");
//    qDebug() << QSerialPortInfo::availablePorts() << endl;
    QList<QSerialPortInfo> serial = QSerialPortInfo::availablePorts();
//    qDebug() << serial.at(0) << endl;
    foreach(const QSerialPortInfo &info, serial)
    {
        qDebug() << "port:\t" << info.portName() << endl;
        ui->serials_box->addItem(info.portName());
    }
    ui->baudrate->addItem("9600");
    ui->baudrate->addItem("115200");
}



void Uart::on_openPortBtn_clicked()
{
    if(ui->serials_box->currentText() == "选择串口" ||
            ui->baudrate->currentText() == "设置波特率"){
        return ;
    }
    serialport = new QSerialPort();//实例化串口类一个对象

    if(serialport->isOpen())//如果串口已经打开了 先给他关闭了
    {
        serialport->clear();
        serialport->close();
    }

    //设置串口名字
    serialport->setPortName(ui->serials_box->currentText());

    if(!serialport->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        QMessageBox::warning(nullptr, tr("串口信息"), tr("串口打开失败"));
        return;
    }

    //打开成功
    ui->openPortBtn->setEnabled(true);
    ui->closePortBtn->setEnabled(true);
    ui->sendBtn->setEnabled(false);
    ui->closeBtn->setEnabled(true);
//    send_message->setEnabled(true);

    //设置波特率和读写方向
    serialport->setBaudRate(ui->baudrate->currentText().toInt() ,QSerialPort::AllDirections);
    serialport->setDataBits(QSerialPort::Data8);      //数据位为8位
    serialport->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    serialport->setParity(QSerialPort::NoParity); //无校验位
    serialport->setStopBits(QSerialPort::OneStop); //一位停止位

    //连接信号槽 当下位机发送数据QSerialPortInfo 会发送个 readyRead 信号,receiveInfo()解析数据
    connect(serialport, SIGNAL(readyRead()), this, SLOT(receiveInfo()));

}

void Uart::on_sendBtn_clicked()
{
    QString data = ui->dataText->text();
    if(data == "")return;
    serialport->write(QString2Hex(data));
}

char Uart::ConvertHexChar(char ch){
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

QByteArray Uart::QString2Hex(QString data){
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = data.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; ){
        hstr=data[i].toLatin1();
        if(hstr == ' '){
            i++;
            continue;
        }
        i++;
        if(i >= len)break;
        lstr = data[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))break;
        else hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}
