#include "pokerserver.h"
#include "ui_pokerserver.h"
#include "mainwindow.h"
#include "info.h"

pokerserver::pokerserver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pokerserver)
{
    ui->setupUi(this);


    server = new ServerStuff(this);
    connect(server, &ServerStuff::gotNewMesssage,
            this, &pokerserver::gotNewMesssage);
    connect(server->tcpServer, &QTcpServer::newConnection,
            this, &pokerserver::smbConnectedToServer);
    connect(server, &ServerStuff::smbDisconnected,
            this, &pokerserver::smbDisconnectedFromServer);
}

pokerserver::~pokerserver()
{
    delete server;
    delete ui;
}



void pokerserver::on_pushButton_startServer_clicked()
{
    if (!server->tcpServer->listen(QHostAddress::Any, portInfo))
    {
        ui->textEdit_log->append(tr("<font color=\"red\"><b>Error!</b> The port is taken by some other service.</font>"));
        return;
    }
    connect(server->tcpServer, &QTcpServer::newConnection, server, &ServerStuff::newConnection);
    ui->textEdit_log->append(tr("<font color=\"green\"><b>Room is opened.</b></font>"));
}

void pokerserver::on_pushButton_stopServer_clicked()
{
    if(server->tcpServer->isListening())
    {
        disconnect(server->tcpServer, &QTcpServer::newConnection, server, &ServerStuff::newConnection);

        QList<QTcpSocket *> clients = server->getClients();
        for(int i = 0; i < clients.count(); i++)
        {
            //server->sendToClient(clients.at(i), "Room closed.");
            server->sendToClient(clients.at(i), "0");
        }

        server->tcpServer->close();
        ui->textEdit_log->append(tr("<b>Room is closed.</b>"));
    }
    else
    {
        ui->textEdit_log->append(tr("<b>Rooom was not opened.</b>"));
    }
}

void pokerserver::on_pushButton_testConn_clicked()
{
    if(server->tcpServer->isListening())
    {
        ui->textEdit_log->append(
                    QString("%1 %2")
                    .arg("Room is opened, number of current players:")
                    .arg(QString::number(server->getClients().count()))
                    );
    }
    else
    {
        ui->textEdit_log->append(
                    QString("%1 %2")
                    .arg("Room is closed, number of current players:")
                    .arg(QString::number(server->getClients().count()))
                    );
    }
}

void pokerserver::smbConnectedToServer()
{
    ui->textEdit_log->append(tr("A new player has joined."));
}

void pokerserver::smbDisconnectedFromServer()
{
    ui->textEdit_log->append(tr("A player has left."));
}

void pokerserver::gotNewMesssage(QString msg)
{
    ui->textEdit_log->append(QString("%1").arg(msg));
}


void pokerserver::on_pushButton_Exit_clicked()
{
    on_pushButton_stopServer_clicked();
    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();
}

void pokerserver::on_pushButton_send_2_clicked()
{
    QList<QTcpSocket *> clients = server->getClients();
    QString msg = ui->lineEdit_message_2->text();

    for(int i = 0; i < clients.count(); i++)
    {
        server->sendToClient(clients.at(i), msg);
    }
    ui->textEdit_log->append(msg);
}

