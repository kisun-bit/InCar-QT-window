#include "music.h"
#include "ui_music.h"
#include "qfiledialog.h"
#include <QDebug>

music::music(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::music)
{
    ui->setupUi(this);
    ui->controlBtn->setEnabled(false);
    ui->controlBtn->setIcon(QIcon(":/new/prefix1/res/start.png"));
    video_timer = new QTimer(this);
    connect(video_timer, SIGNAL(timeout()), this, SLOT(do_timer()));


}

music::~music()
{
//    Ui::music *ui;
//    QProcess *process;
//    QMediaPlayer *player;
//    QStringList  playList;
//    int status=-1;
    delete player;
    delete process;
    delete ui;
}


//choose music file
void music::on_file_clicked()
{

    QStringList  arr_list = QFileDialog::getOpenFileNames(this,
                                  "需要播放的文件(mp3)",
                                  "/",
                                  "Song (*.mp3)");
    playList.clear();
    if (arr_list.count() == 0){
        return;
    }
    playList = arr_list;
    playMedia(playList.at(0));
}


void music::playMedia(QString filename){
    ui->controlBtn->setEnabled(true);
    ui->controlBtn->setIcon(QIcon(":/new/prefix1/res/stop.png"));
    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(doProcessStd()));
    ui->label->setText(filename);
    QStringList args;
    args << "-slave";
    args << "-quiet";
    args << "-zoom";
    args << "-x";
    args << "240";
    args << "-y";
    args << "180";
    args << filename;
    process->start("mplayer", args);
    status = 1;
    video_timer->start(500);
}

void music::on_controlBtn_clicked()
{
    QString cmd="pause\n";
    QString icon;
    if (status==1){
        // to stop
        status = 2;
        icon = ":/new/prefix1/res/start.png";
    }
    else if(status ==2 ){
        status = 1;
        icon = ":/new/prefix1/res/stop.png";
    }
    ui->controlBtn->setIcon(QIcon(icon));
    process->write(cmd.toLatin1());
}

//void music::on_process_sliderMoved(int position)
//{
//    process->write(QString("seek "+QString::number(position)+" 2\n").toUtf8());
//}

void music::on_process_valueChanged(int value)
{
    if(status == 1 || status == 2){
        QString cmd = QString("seek %1 1\n").arg(value);
//        qDebug() << cmd << endl;
        process->write(cmd.toLatin1());
    }
}

void music::do_timer(){
    QString cmd = "get_percent_pos\n";
    process->write(cmd.toLatin1());
}

void music::doProcessStd(){
    QString str=QString(process->readAll());
    if(str.contains("ANS_PERCENT_POSITION")){
        str.replace("\n", "");
        QString var = str.split("=").at(1);
        ui->process->setValue(var.toInt());
    }
}

void music::on_pushButton_clicked()
{
    delete player;
    delete process;
    delete ui;
    this->close();
}
