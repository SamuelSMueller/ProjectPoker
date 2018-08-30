#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class createroom;
}

class createroom : public QDialog
{
    Q_OBJECT

public:
    explicit createroom(QWidget *parent = nullptr);
    ~createroom();
    void setUsername(QString uName);

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    QString password;
    QString username;
    Ui::createroom *ui;
};

#endif // CREATEROOM_H
