#ifndef POKERSERVER_H
#define POKERSERVER_H

#include <QDialog>
#include <QDebug>
#include <QObject>
#include <QRandomGenerator>
#include <QTime>
#include <QtCharts>


#include "serverStuff.h"

namespace Ui {
class pokerserver;
}

class pokerserver : public QDialog
{
    Q_OBJECT

public:
    explicit pokerserver(QWidget *parent = nullptr);
    ~pokerserver();
    void setUsername(QString username);
    void setPassword(QString pass);

private slots:
    void on_pushButton_stopServer_clicked();
    void on_pushButton_startServer_clicked();
    void on_pushButton_testConn_clicked();
    void smbConnectedToServer();
    void smbDisconnectedFromServer();
    void gotNewMesssage(QString msg, QTcpSocket *clientSocket);
    void on_pushButton_Exit_clicked();
    void on_pushButton_send_2_clicked();
    void on_pushButton_Shuffle_clicked();
    void makechart(QList<QBarSeries *> series);

    void on_pushButton_Reset_clicked();

private:
    QString username;
    QList<QString> currentUsers;
    Ui::pokerserver *ui;
    ServerStuff *server;
    QList<QString> userVotes;
    QString password;
    bool voteComplete;
    bool newProgress;
};

#endif // POKERSERVER_H
