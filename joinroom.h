#ifndef JOINROOM_H
#define JOINROOM_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class joinroom;
}

class joinroom : public QDialog
{
    Q_OBJECT

public:
    explicit joinroom(QWidget *parent = nullptr);
    ~joinroom();
    void setUsername(QString uName);

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_checkBox_stateChanged(int arg1);

private:
    QString username;
    Ui::joinroom *ui;
    QString password;
};

#endif // JOINROOM_H
