#ifndef POKERSERVER_H
#define POKERSERVER_H

#include <QDialog>
#include <QDebug>
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

private slots:
    void on_pushButton_stopServer_clicked();
    void on_pushButton_startServer_clicked();
    void on_pushButton_testConn_clicked();
    void smbConnectedToServer();
    void smbDisconnectedFromServer();
    void gotNewMesssage(QString msg);

private:
    Ui::pokerserver *ui;
    ServerStuff *server;
};

#endif // POKERSERVER_H
