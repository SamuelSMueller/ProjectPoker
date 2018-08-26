#include "pokerserver.h"
#include "ui_pokerserver.h"
#include "mainwindow.h"
#include "info.h"

pokerserver::pokerserver(QWidget *parent,  QString rName) :
    QDialog(parent),
    ui(new Ui::pokerserver)
{
    ui->setupUi(this);
    roomname = rName;
    server = new ServerStuff(this, roomname);
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
    ui->textEdit_log->append(QString("<font color=\"green\"><b> %1 %2 </b></font>")
                .arg(roomname)
                .arg(tr(" is opened."))
                );
    ui->listWidget->addItem("Admin: " + username);
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
        ui->textEdit_log->append(QString("<font color=\"green\"><b> %1 %2 </b></font>")
                                 .arg(roomname)
                                 .arg(tr(" is closed."))
                                 );

        currentUsers.clear();
        ui->listWidget->clear();

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
                    .arg(QString::number(server->getClients().count()+1))
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
  ui->textEdit_log->append("A new player has joined.");
}

void pokerserver::smbDisconnectedFromServer()
{
    ui->textEdit_log->append("A player has left.");
}

void pokerserver::gotNewMesssage(QString msg, QTcpSocket *clientSocket)
{
    if(msg.startsWith("/USER/"))
    {
        qDebug()<<"CHECK 1";
        msg.remove(0,6);
        if(currentUsers.isEmpty())
        {
            currentUsers.append("Admin: "+username);
        }

        int j = currentUsers.indexOf(("Player: "+msg));
        QList<QTcpSocket *> clients = server->getClients();
        qDebug()<<"CHECK 2";
        if(j == -1)
        {
           currentUsers.append("Player: "+msg);
        }

        else if(j != -1)
        {
            server->sendToClient(clientSocket, "/ERRNAME/");
        }

        ui->listWidget->clear();

        for(int i = 0; i<currentUsers.count(); i++)
        {
            ui->listWidget->addItem(currentUsers.at(i));
        }

        for(int i = 0; i < clients.count(); i++)
        {
            server->sendToClient(clients.at(i), "/CLEARLIST/");
            for(int j = 0; j < currentUsers.count(); j++)
            {
                server->sendToClient(clients.at(i), ("/ADDLIST/"+currentUsers.at(j)));
            }
            server->sendToClient(clients.at(i), "/ENDLIST/");
        }
    }
    else if(msg.startsWith("/DCON/"))
    {
        msg.remove(0,6);
        QList<QListWidgetItem*> names = ui->listWidget->findItems(("Player: "+msg) , Qt::MatchExactly);
        if(names.count() > 0)
        {
            delete ui->listWidget->takeItem(ui->listWidget->row(names.first()));
            int j = currentUsers.indexOf(("Player: "+msg));
            if(j != -1)
            {
                currentUsers.removeAt(j);
            }
        names.clear();
        }

        QList<QTcpSocket *> clients = server->getClients();
        for(int i = 0; i < clients.count(); i++)
        {
            server->sendToClient(clients.at(i), "/CLEARLIST/");
            for(int j = 0; j < currentUsers.count(); j++)
            {
                server->sendToClient(clients.at(i), ("/ADDLIST/"+currentUsers.at(j)));
            }
            server->sendToClient(clients.at(i), "/ENDLIST/");
        }
    }
    else
    {
        ui->textEdit_log->append(QString("%1").arg(msg));
    }
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
    QString msg = username + ": " + (ui->lineEdit_message_2->text());


    for(int i = 0; i < clients.count(); i++)
    {
        server->sendToClient(clients.at(i), msg);
    }
    ui->textEdit_log->append(msg);
}

void pokerserver::setUsername(QString uName)
{
    username = uName;
}

void pokerserver::on_pushButton_Shuffle_clicked()
{   QTime time = QTime::currentTime();

    QRandomGenerator rand((uint)time.msec());
    int gen;
    int first = 1;
    int last = currentUsers.count();
    for(int i = 1; i<currentUsers.count(); i++)
    {
        qDebug()<<"Current Index: ";
        qDebug()<<i;

        gen = rand.bounded(first, last);

        qDebug()<<"Random Num: ";
        qDebug()<<gen;
        currentUsers.swap(i, gen);
    }

    ui->listWidget->clear();

    for(int i = 0; i<currentUsers.count(); i++)
    {
        ui->listWidget->addItem(currentUsers.at(i));
    }

    QList<QTcpSocket *> clients = server->getClients();
    for(int i = 0; i < clients.count(); i++)
    {
        server->sendToClient(clients.at(i), "/CLEARLIST/");
        for(int j = 0; j < currentUsers.count(); j++)
        {
            server->sendToClient(clients.at(i), ("/ADDLIST/"+currentUsers.at(j)));
        }
        server->sendToClient(clients.at(i), "/ENDLIST/");
    }
}
