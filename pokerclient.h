#ifndef POKERCLIENT_H
#define POKERCLIENT_H

#include <QDialog>
#include <QAbstractSocket>
#include "clientStuff.h"

namespace Ui {
class pokerclient;
}

class pokerclient : public QDialog
{
    Q_OBJECT

public:
    explicit pokerclient(QWidget *parent = nullptr, QString uName = "username");
    ~pokerclient();
    void setRoomname(QString roomname);

public slots:
    void setStatus(bool newStatus);
    void receivedSomething(QString msg);
    void gotError(QAbstractSocket::SocketError err);

private slots:
    void on_pushButton_send_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_disconnect_clicked();
    void on_pushButton_Exit_clicked();

private:
    QString username;
    QString roomname;
    QList<QString> currentUsers;
    Ui::pokerclient *ui;
    ClientStuff *client;
};

#endif // POKERCLIENT_H
