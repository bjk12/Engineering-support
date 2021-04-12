#ifndef DENGLU_H
#define DENGLU_H

#include <QDialog>

namespace Ui {
class denglu;
}

class denglu : public QDialog
{
    Q_OBJECT

public:
    explicit denglu(QWidget *parent = 0);
    ~denglu();

    void WriteInit(QString path, QString user_key, QString &user_value);
    void ReadInit(QString path, QString user_key, QString &user_value);

private slots:
    void on_okButton_clicked();

    void on_exitButton_clicked();
    void on_password_returnPressed();

signals:
    void success();
private:
    Ui::denglu *ui;
    void reject();
};

#endif // DENGLU_H
