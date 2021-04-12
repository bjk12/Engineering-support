#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <denglu.h>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_countButton_clicked();

    void on_re0dButton_clicked();

    void on_yesButton_clicked();

    void on_stopButton_clicked();

    void on_timeout(); //定时溢出处理槽函数

    void on_saveButton_clicked();

    void on_clearButton_clicked();

    void mainwind_show();

    void on_save2Button_clicked();

    void on_save3Button_clicked();

    void on_videoButton_clicked();

    void on_loadButton_clicked();

    void on_revideoButton_clicked();

    void on_songnumber_sliderMoved(int position);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    QProgressBar* m_pProgressBar;
    QTimer* Timer1;
    denglu* wind1;
    QMediaPlayer *mediaPlayer;
};

#endif // MAINWINDOW_H
