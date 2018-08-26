#ifndef POKERSERVER_H
#define POKERSERVER_H

#include <QDialog>
#include <QDebug>
#include <QObject>
#include "serverStuff.h"

namespace Ui {
class pokerserver;
}

class pokerserver : public QDialog
{
    Q_OBJECT

public:
    explicit pokerserver(QWidget *parent = nullptr, QString rName = "roomname");
    ~pokerserver();
    void setUsername(QString username);
    void setRoomname(QString roomname);

private slots:
    void on_pushButton_stopServer_clicked();
    void on_pushButton_startServer_clicked();
    void on_pushButton_testConn_clicked();
    void smbConnectedToServer();
    void smbDisconnectedFromServer();
    void gotNewMesssage(QString msg);

    void on_pushButton_Exit_clicked();

    void on_pushButton_send_2_clicked();

private:
    QString username;
    QString roomname;
    QList<QString> currentUsers;
    Ui::pokerserver *ui;
    ServerStuff *server;
};

#endif // POKERSERVER_H
