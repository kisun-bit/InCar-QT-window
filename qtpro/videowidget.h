#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QProcess> // 后台进程播放
#include <QtMultimedia>
#include <QtMultimediaWidgets>

namespace Ui {
class VideoWidget;
}

class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

private slots:
    void on_btn_openfile_clicked();

    void on_btn_start_clicked();

    void on_btn_next_clicked();

    void on_btn_back_clicked();

//    void on_dial_volice_actionTriggered(int action);

    void on_dial_volice_valueChanged(int value);

    void on_horizontalSlider_valueChanged(int value);

//    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_clicked();

private:
    Ui::VideoWidget *ui;
    QStringList  playList;
    QProcess *playProcess;//播放进程
    int isPlaying=0;   //0:init   1:playing   2:stop    4:end
    int playIndex=0;//播放下标
    QMediaPlayer* player;

    void init();
    void playMovie(const QString filename);


};

#endif // VIDEOWIDGET_H
