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
        QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\bjk",QSettings::NativeFormat);
//      QStringList val = reg->allKeys();// 如果此键不存在，则返回的是空字符串
        if(reg->contains("bjk9_name"))
        {
            QString bjk9_name=reg->value("bjk9_name").toString();
            ui->name1->setText(bjk9_name);
            ui->jizhu_password->setChecked(true);
        }
        else
            ui->jizhu_password->setChecked(false);
        if(reg->contains("bjk9_password"))
        {
            QString bjk9_password=reg->value("bjk9_password").toString();
            ui->password->setText(bjk9_password);
        }

        QString appName = QApplication::applicationName();//程序名称
        //换成 HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run则不需要管理员权限
        reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
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
            QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\bjk",QSettings::NativeFormat);
            if(reg->contains("bjk9_name"))
            {
                reg->remove("bjk9_name");
            }
            reg->setValue("bjk9_name",ui->name1->text());
            if(reg->contains("bjk9_password"))
            {
                reg->remove("bjk9_password");
            }
            reg->setValue("bjk9_password",ui->password->text());
            reg->deleteLater();
        }
        else
        {
            QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\bjk",QSettings::NativeFormat);
            reg->clear();
//            if(reg->contains("bjk9_name"))
//            {
//                reg->remove("bjk9_name");
//            }
//            if(reg->contains("bjk9_password"))
//            {
//                reg->remove("bjk9_password");
//            }
            reg->deleteLater();
        }
        if(ui->auto_start->isChecked())
        {
            //以下是开机自启动程序
            QString appName = QApplication::applicationName();//程序名称
            QString appPath = QApplication::applicationFilePath();// 程序路径
            appPath = appPath.replace("/","\\");
            QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
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
            QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
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
