#ifndef POKERCLIENT_H
#define POKERCLIENT_H

#include <QDialog>
#include <QAbstractSocket>
#include <QMessageBox>
#include <QtCharts>
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


    void on_pushButton_Vote8_clicked();
    void on_pushButton_Vote7_clicked();
    void on_pushButton_Vote6_clicked();
    void on_pushButton_Vote5_clicked();
    void on_pushButton_Vote4_clicked();
    void on_pushButton_Vote3_clicked();
    void on_pushButton_Vote2_clicked();
    void on_pushButton_Vote1_clicked();
    void on_pushButton_Vote_clicked();
    void makechart(QList<QBarSeries *> series);

private:
    QString voteNum;
    QString username;
    QString roomname;
    QList<QString> currentUsers;
    Ui::pokerclient *ui;
    ClientStuff *client;
    QList<QString> userVotes;
};

#endif // POKERCLIENT_H
