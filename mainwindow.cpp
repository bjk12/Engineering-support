#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
int nScreenWidth, nScreenHeight;
int NW=0,NH=0,time11,time22,time0,flagt=0,savetxt=0,timeall=0,timenow=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint);
    //去掉最大化最小化按钮，保留关闭按钮
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    //    printf("屏幕分辨率） 宽：%d 高：%d\n", DevMode.dmPelsWidth,DevMode.dmPelsHeight);
    //    printf("屏幕大小（毫米） 宽：%d 高：%d\n", nScreenWidth, nScreenHeight);
    //    double fDiagonalLen = sqrt(nScreenHeight * nScreenHeight + nScreenWidth * nScreenWidth);
    //    printf("屏幕对角线长为：%.2lf毫米 约 %.2lf英寸\n", fDiagonalLen, fDiagonalLen * MILLIMETRE_TO_INCH);
    DEVMODE DevMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);
    NW=DevMode.dmPelsWidth;
    NH=DevMode.dmPelsHeight;
    HDC hdcScreen = GetDC(NULL);   //获取屏幕的HDC
    nScreenWidth = GetDeviceCaps(hdcScreen, HORZSIZE);
    nScreenHeight = GetDeviceCaps(hdcScreen, VERTSIZE);
    m_pProgressBar = ui->progressBar;
    m_pProgressBar->setOrientation(Qt::Horizontal);  // 水平方向
    m_pProgressBar->setMinimum(0);  // 最小值
    m_pProgressBar->setMaximum(1500);  // 最大值
    m_pProgressBar->setValue(0);  // 当前进度
    double dProgress=0;
//    double dProgress = (m_pProgressBar->value() - m_pProgressBar->minimum()) * 100.0
//                    / (m_pProgressBar->maximum() - m_pProgressBar->minimum()); // 百分比计算公式
    m_pProgressBar->setFormat(QString("当前长度为：%1mm").arg(QString::number(dProgress, 'f', 1)));
//    m_pProgressBar->setFormat(tr("Current progress : %1%").arg(QString::number(dProgress, 'f', 1)));
    m_pProgressBar->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 对齐方式

    ui->donetimeEdit->setReadOnly(true);
    ui->donetimeEdit->setText("0");
    Timer1=new QTimer(this);
    Timer1->stop();
    Timer1->setInterval (1000) ;
    ui->stopButton->setEnabled(false);
    wind1=new denglu(this);
    wind1->setModal(false);
    wind1->show();
    connect(Timer1,SIGNAL(timeout()),this,SLOT(on_timeout()));
    connect(wind1,SIGNAL(success()),this,SLOT(mainwind_show()));

    mediaPlayer = new QMediaPlayer(this);
    //设置mediaPlayer的QVideoWidget播放窗口
//    mediaPlayer->setVideoOutput(ui->widget);
    ui->songEdit->setReadOnly(true);
    connect(mediaPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(durationChanged(qint64)));
    connect(mediaPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
    double rule_range=1500*nScreenWidth/NW/0.998;
    ui->label->setText(tr("真实长度（单位：mm，量程：")+QString("%1").arg(rule_range-1, 2, 10,0,QChar(' '))+tr("mm）："));
    ui->doubleSpinBox->setRange(0,rule_range-1);
    this->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_countButton_clicked()
{
    int x=ui->donetimeEdit->text().toInt();
    x=x+1;
    ui->donetimeEdit->setText(QString::number(x,10));
}

void MainWindow::on_re0dButton_clicked()
{
    ui->donetimeEdit->setText("0");
}

void MainWindow::on_yesButton_clicked()
{
    ui->yesButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    time11=ui->workEdit->text().toInt();
    time22=ui->restEdit->text().toInt();
    QTime T1=QTime::currentTime();
    time0=3600*T1.hour()+60*T1.minute()+T1.second();
    Timer1->start();
}

void MainWindow::on_stopButton_clicked()
{
    Timer1->stop();
    ui->stopButton->setEnabled(false);
    ui->yesButton->setEnabled(true);
}
void MainWindow::on_timeout()
{
    if(flagt==0)
    {
        QTime T1=QTime::currentTime();
        if((3600*T1.hour()+60*T1.minute()+T1.second()-time0)>=60*time11)
        {
            flagt=1;
            time0=3600*T1.hour()+60*T1.minute()+T1.second();
            QMessageBox::about(this, "温馨提示", "工作辛苦了，请您站起来伸个懒腰，出去散散步，休息一下吧。");
        }
    }
    else
    {
        QTime T1=QTime::currentTime();
        if((3600*T1.hour()+60*T1.minute()+T1.second()-time0)>=60*time22)
        {
            flagt=0;
            time0=3600*T1.hour()+60*T1.minute()+T1.second();
            QMessageBox::about(this, "温馨提示", "欢迎您回来继续工作。");
        }
    }
}

void MainWindow::on_saveButton_clicked()
{
    int x=ui->donetimeEdit->text().toInt();
    QString recv = ui->recEdit->toPlainText()+QString("%1").arg(x, 6, 10, QChar(' '));
    ui->recEdit->setPlainText(recv);
    ui->donetimeEdit->setText("0");
}

void MainWindow::on_clearButton_clicked()
{
    ui->recEdit->clear();
}

void MainWindow::mainwind_show()
{
    this->show();
}

void MainWindow::on_save2Button_clicked()
{
    int x=ui->donetimeEdit->text().toInt();
    QString recv = ui->recEdit->toPlainText()+QString("%1").arg(x, 6, 10, QChar(' '));
    ui->recEdit->setPlainText(recv);
}

void MainWindow::on_save3Button_clicked()
{
    savetxt++;
    QString recv = ui->recEdit->toPlainText();
    QString location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QFile file1(location+"\\"+QString("%1").arg(savetxt, 0, 10)+".txt");
    file1.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream txtOutput(&file1);
    txtOutput <<recv<< endl;
    file1.close();
}

void MainWindow::on_videoButton_clicked()
{
    switch (mediaPlayer->state())
    {
        case QMediaPlayer::PlayingState:
            mediaPlayer->pause();
            ui->videoButton->setText(tr("播放"));
            break;
        default:
            mediaPlayer->play();
            ui->videoButton->setText(tr("暂停"));
            break;
    }
}
void MainWindow::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("打开视频/音频"),"./",tr("*.wmv *.avi *.mp3"));
    //设置播放内容
    mediaPlayer->setMedia(QUrl::fromLocalFile(filename));
    ui->songEdit->setText(filename.section("/",-1,-1).section(".",0,0));
    mediaPlayer->pause();
    ui->videoButton->setText(tr("播放"));
}

void MainWindow::on_revideoButton_clicked()
{
    mediaPlayer->setPosition(0);
    mediaPlayer->play();
}

void MainWindow::on_songnumber_sliderMoved(int position)
{
    timenow=position/1000;
    mediaPlayer->setPosition(position);
    ui->labeltt->setText(QString("%1").arg(timenow/60, 2, 10,QChar('0'))+":"+QString("%1").arg(timenow%60, 2, 10,QChar('0'))
                         +"/"+QString("%1").arg(timeall/60, 2, 10,QChar('0'))+":"+QString("%1").arg(timeall%60, 2, 10,QChar('0')));
}

void MainWindow::durationChanged(qint64 duration)
{
    timeall=duration/1000;
    ui->songnumber->setRange(0,duration);
    ui->labeltt->setText(QString("%1").arg(timenow/60, 2, 10,QChar('0'))+":"+QString("%1").arg(timenow%60, 2, 10,QChar('0'))
                         +"/"+QString("%1").arg(timeall/60, 2, 10,QChar('0'))+":"+QString("%1").arg(timeall%60, 2, 10,QChar('0')));
}

void MainWindow::positionChanged(qint64 position)
{
    timenow=position/1000;
    ui->songnumber->setValue(position);
    ui->labeltt->setText(QString("%1").arg(timenow/60, 2, 10,QChar('0'))+":"+QString("%1").arg(timenow%60, 2, 10,QChar('0'))
                         +"/"+QString("%1").arg(timeall/60, 2, 10,QChar('0'))+":"+QString("%1").arg(timeall%60, 2, 10,QChar('0')));
}

void MainWindow::on_doubleSpinBox_valueChanged(double m1)
{
//    QString m1=ui->lineEdit->text();
//    if(m1.isEmpty())
//    {
//        ui->lineEdit->setText("请输入数据。");
//        return;
//    }
//    int nCount = m1.count();
//    for(int i = 0 ; i < nCount ; i++)
//    {
//        QChar cha = m1.at(i);
//        ushort uni = cha.unicode();
//        if(uni >= 0x4E00 && uni <= 0x9FA5)
//        {
//            //这个字符是中文
//            ui->lineEdit->setText("汉字无效！");
//            return;
//        }
//    }
//    if(m1.toDouble()==0 && m1.compare("0")!=0
//            && m1.compare("0.")!=0
//            && m1.compare("0.0")!=0
//            && m1.compare("0.00")!=0
//            && m1.compare("0.000")!=0
//            && m1.compare("0.0000")!=0
//            && m1.compare("0.00000")!=0
//            && m1.compare("0.000000")!=0
//            && m1.compare("0.0000000")!=0
//            && m1.compare("0.00000000")!=0
//            && m1.compare("0.000000000")!=0
//            && m1.compare("0.0000000000")!=0
//            && m1.compare("")!=0
//            )
//    {
////        QApplication::setQuitOnLastWindowClosed(false);
//        QMessageBox::about(NULL, "温馨提示", "请输入有意义的数字。");
//        return;
//    }
    double truelong=0.998*m1*NW/nScreenWidth;
//    qDebug()<<truelong;
    if(truelong<=1500)
    {
        m_pProgressBar->setValue(truelong);  // 当前进度
        m_pProgressBar->setFormat(QString("当前长度为：%1mm").arg(QString::number(m1, 'f', 2)));
    }
    else
    {
        QMessageBox::about(NULL, "温馨提示", "对不起，用户所求长度超出软件最大量程。");
        truelong=0;
        m_pProgressBar->setValue(truelong);  // 当前进度
        m_pProgressBar->setFormat(QString("当前长度为：%1mm").arg(QString::number(truelong, 'f', 2)));
    }
}
