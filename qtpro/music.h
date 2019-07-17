#ifndef MUSIC_H
#define MUSIC_H

#include <QWidget>
#include <QProcess>
#include <QtMultimedia>
#include <QTimer>

namespace Ui {
class music;
}

class music : public QWidget
{
    Q_OBJECT

public:
    explicit music(QWidget *parent = nullptr);
    ~music();
    void playMedia(QString filename);
    QTimer *video_timer;

private slots:
    void on_file_clicked();

    void on_controlBtn_clicked();

//    void on_process_sliderMoved(int position);

    void on_process_valueChanged(int value);
    void do_timer();
    void doProcessStd();

    void on_pushButton_clicked();

private:
    Ui::music *ui;
    QProcess *process;
    QMediaPlayer *player;
    QStringList  playList;
    int status=-1;
};

#endif // MUSIC_H
