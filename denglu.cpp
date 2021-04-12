#include "denglu.h"
#include "ui_denglu.h"
#include <QSettings>
denglu::denglu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::denglu)
{
    ui->setupUi(this);
    Qt::WindowFlags flags= this->windowFlags();
    setWindowFlags(flags&~(
                           Qt::WindowContextHelpButtonHint|
                           Qt::WindowMaximizeButtonHint|
                           Qt::WindowMinimizeButtonHint|
                           Qt::WindowCloseButtonHint)
                   );
        QString readIP="";
        QString readPort="";
        QString readState="";
        ReadInit(QString("./user.ini"),"IP", readIP);
        ReadInit(QString("./user.ini"),"PORT", readPort);
        ReadInit(QString("./user.ini"),"STATE", readState);
        ui->jizhu_password->setChecked(false);
        if(!readState.compare("checked")){
            ui->jizhu_password->setChecked(true);
        }
        ui->name1->setText(readIP);
        ui->password->setText(readPort);


        QString appName = QApplication::applicationName();//程序名称
        //换成 HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run则不需要管理员权限
        QSettings *reg=new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
        if(reg->contains(appName))
        {
            ui->auto_start->setChecked(true);
        }
        else
            ui->auto_start->setChecked(false);
//      reg->setValue(appName,appPath);// 如果移除的话，reg->remove(applicationName);
        reg->deleteLater();
}

denglu::~denglu()
{
    delete ui;
}

void denglu::WriteInit(QString path, QString user_key, QString &user_value)
{
    if (!path.isEmpty() && !user_key.isEmpty())
    {
        //创建配置文件操作对象
        QSettings *config = new QSettings(path, QSettings::IniFormat);

        //将信息写入配置文件
        config->beginGroup("config");
        config->setValue(user_key, user_value);
        config->endGroup();
        delete config;

    }
}

void denglu::ReadInit(QString path, QString user_key, QString &user_value)
{
    user_value = QString("");
    if (!path.isEmpty() && !user_key.isEmpty())
    {
        //创建配置文件操作对象
        QSettings *config = new QSettings(path, QSettings::IniFormat);

        //读取用户配置信息
        user_value = config->value(QString("config/") + user_key).toString();

        delete config;
    }
}

void denglu::on_okButton_clicked()
{
    if(((ui->name1->text()==QString("启蒙"))&&(ui->password->text()==QString("321930")))||
            ((ui->name1->text()==QString("祁萌"))&&(ui->password->text()==QString("123")))
      )
    {
//        QApplication::setQuitOnLastWindowClosed(false);//防止消息框退出界面
        QMessageBox::about(this, "温馨提示", "身份确认，欢迎您使用本软件。");
        if(ui->jizhu_password->isChecked())
        {
            QString ip=ui->name1->text();
            QString port=ui->password->text();
            QString state="checked";
            WriteInit(QString("./user.ini"),"IP",ip);
            WriteInit(QString("./user.ini"),"PORT",port);
            WriteInit(QString("./user.ini"),"STATE",state);
        }
        else
        {
            QString empty1=QString("");
            WriteInit(QString("./user.ini"),"IP",empty1);
            WriteInit(QString("./user.ini"),"PORT",empty1);
            WriteInit(QString("./user.ini"),"STATE",empty1);
        }
        if(ui->auto_start->isChecked())
        {
            //以下是开机自启动程序
            QString appName = QApplication::applicationName();//程序名称
            QString appPath = QApplication::applicationFilePath();// 程序路径
            appPath = appPath.replace("/","\\");
            QSettings *reg=new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
            if(reg->contains(appName))
            {
                reg->remove(appName);
            }
            reg->setValue(appName,appPath);
            reg->deleteLater();
        }
        else
        {
            QString appName = QApplication::applicationName();//程序名称
            QString appPath = QApplication::applicationFilePath();// 程序路径
            appPath = appPath.replace("/","\\");
            QSettings *reg=new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
            if(reg->contains(appName))
            {
                reg->remove(appName);
            }
            reg->deleteLater();
        }
        emit success();
        this->accept();
    }
    else
    {
//        QApplication::setQuitOnLastWindowClosed(false);//防止消息框退出界面
        QMessageBox::about(this, "温馨提示", "账号或密码输入错误。");
    }
}

void denglu::on_exitButton_clicked()
{
    this->accept();
}

void denglu::reject()
{
}

void denglu::on_password_returnPressed()
{
    if(!ui->name1->text().isEmpty())
    {
        on_okButton_clicked();
    }
}
