#include "videowidget.h"
#include "ui_videowidget.h"
#include <QDebug>

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoWidget)
{
    ui->setupUi(this);
    init();
}

VideoWidget::~VideoWidget()
{
    delete player;
    delete playProcess;
    delete ui;
}

void VideoWidget::on_btn_openfile_clicked()
{
    // 选中视频文件为列表
    QStringList  arr = QFileDialog::getOpenFileNames(this,
                                  "需要播放的文件",
                                  ".",
                                  "video (*.mp4 *.avi *.rmvb)");
//    qDebug() << 111 << endl;
    // 未选择视频文件则直接返回
    if(arr.length() <= 0){
        return;
    }

    playList = arr;
    playIndex = 0; // 选择多个视频文件只播放第一个
    playMovie(playList.at(playIndex));
}

void VideoWidget::init(){
    playList.clear();
    playIndex = 0;
    isPlaying = -1;
    playProcess = new QProcess();
}


void VideoWidget::playMovie(const QString filename){
    playProcess->close();
    QString program = "mplayer";
    QStringList arguments;
    arguments << filename;
     //(mplayer -gui-wid<window_id>)  can play video in a gun widget
    arguments << "-wid";
    arguments << QString::number(ui->label_show->winId());//parse to string
    arguments << "-slave";
    arguments << "-zoom";
    arguments << "-quiet";
    playProcess->start(program, arguments);
    isPlaying = 1;
    ui->btn_start->setIcon(QIcon(":/new/prefix1/res/stop.png"));
    QString cmd = QString("volumn %1 1\n").arg(ui->dial_volice->value());
//            qDebug() << cmd << endl;
    playProcess->write(cmd.toLatin1());
    ui->horizontalSlider->setValue(0);
}


void VideoWidget::on_btn_start_clicked()
{
    if(isPlaying==-1){
        return;
    }
    QString cmd="pause\n";
    if(isPlaying == 1){
        //play=>stop
        isPlaying=2;
        ui->btn_start->setIcon(QIcon(":/new/prefix1/res/start.png"));
//        cmd = "pause\n";
    }
    else if(isPlaying == 2)
    {
        isPlaying=1;
        //stop=>start
//        cmd = "play\n";
        ui->btn_start->setIcon(QIcon(":/new/prefix1/res/stop.png"));
    }
    playProcess->write(cmd.toLatin1());
}

//
void VideoWidget::on_btn_next_clicked()
{
    if (playIndex > 0){
        playIndex--;
        playMovie((playList.at(playIndex)));
    }
}


void VideoWidget::on_btn_back_clicked()
{
    if (playIndex < playList.length()-1){
        playIndex++;
        playMovie((playList.at(playIndex)));
    }
}

//void VideoWidget::on_dial_volice_actionTriggered(int action)
//{
//    qDebug() << action;
//    if(isPlaying == 1 || isPlaying == 2){
//        QString cmd = "volumn "+QString::number(action)+" 1\n";
//        qDebug() << cmd << endl;
//        playProcess->write(cmd.toLantin1());
//    }
//}

void VideoWidget::on_dial_volice_valueChanged(int value)
{
//        qDebug() << value;
        if(isPlaying == 1 || isPlaying == 2){
            QString cmd = QString("volumn %1 1\n").arg(value);
//            qDebug() << cmd << endl;
            playProcess->write(cmd.toLatin1());
        }
}

void VideoWidget::on_horizontalSlider_valueChanged(int value)
{
    if(isPlaying == 1 || isPlaying == 2){
        QString cmd = QString("seek %1 1\n").arg(value);
//        qDebug() << cmd << endl;
        playProcess->write(cmd.toLatin1());
    }
}

//void VideoWidget::on_horizontalSlider_sliderMoved(int position)
//{
//    playProcess->write(QString("seek "+QString::number(position)+" 2\n").toUtf8());
//}

void VideoWidget::on_pushButton_clicked()
{
    this->close();
}
