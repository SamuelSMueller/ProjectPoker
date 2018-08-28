#include "pokerserver.h"
#include "ui_pokerserver.h"
#include "mainwindow.h"
#include "info.h"

pokerserver::pokerserver(QWidget *parent,  QString rName) :
    QDialog(parent),
    ui(new Ui::pokerserver)
{
    ui->setupUi(this);
    QList<QBarSeries *> series;
    makechart(series);
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
    else if(msg.startsWith("/VOTEN/"))
    {
        msg.remove(0,7);
        userVotes.append(msg);
        qDebug()<<userVotes[0];
        qDebug()<<msg;
        if(userVotes.count() == (currentUsers.count()-1))
        {
            //------WRITTEN TERRIBLY, LATE AT NIGHT, AFTER HOMEWORK. WILL BE REWRITTEN LATER--------//
            int first = 0;
            int second = 0;
            int third = 0;
            int fourth = 0;
            int fifth = 0;
            int sixth = 0;
            int seventh = 0;
            int eigth = 0;

            QBarSet *set0 = new QBarSet("?");
            QBarSet *set1 = new QBarSet("0");
            QBarSet *set2 = new QBarSet("1/2");
            QBarSet *set3 = new QBarSet("1");
            QBarSet *set4 = new QBarSet("2");
            QBarSet *set5 = new QBarSet("3");
            QBarSet *set6 = new QBarSet("5");
            QBarSet *set7 = new QBarSet("8");

            for(int i = 0; i<userVotes.count(); i++)
            {
                if(userVotes.at(i) == "?")
                {
                    first++;
                }
                else if(userVotes.at(i) == "0")
                {
                    second++;
                }
                else if(userVotes.at(i) == "1/2")
                {
                    third++;
                }
                else if(userVotes.at(i) == "1")
                {
                    fourth++;
                }
                else if(userVotes.at(i) == "2")
                {
                    fifth++;
                }
                else if(userVotes.at(i) == "3")
                {
                    sixth++;
                }
                else if(userVotes.at(i) == "5")
                {
                    seventh++;
                }
                else if(userVotes.at(i) == "8")
                {
                    eigth++;
                }

                qDebug()<<userVotes.at(i);
            }

            *set0<<first;
            *set1<<second;
            *set2<<third;
            *set3<<fourth;
            *set4<<fifth;
            *set5<<sixth;
            *set6<<seventh;
            *set7<<eigth;

            QBarSeries *series0 = new QBarSeries();
            QBarSeries *series1 = new QBarSeries();
            QBarSeries *series2 = new QBarSeries();
            QBarSeries *series3 = new QBarSeries();
            QBarSeries *series4 = new QBarSeries();
            QBarSeries *series5 = new QBarSeries();
            QBarSeries *series6 = new QBarSeries();
            QBarSeries *series7 = new QBarSeries();

            QList<QBarSeries *> allSeries;
            series0->append(set0);
            allSeries.append(series0);
            series1->append(set1);
            allSeries.append(series1);
            series2->append(set2);
            allSeries.append(series2);
            series3->append(set3);
            allSeries.append(series3);
            series4->append(set4);
            allSeries.append(series4);
            series5->append(set5);
            allSeries.append(series5);
            series6->append(set6);
            allSeries.append(series6);
            series7->append(set7);
            allSeries.append(series7);

            makechart(allSeries);
//----------------------REWRITE ABOVE----------------------//
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
        gen = rand.bounded(first, last);

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






void pokerserver::makechart(QList<QBarSeries *> allSeries)
{
    userVotes.clear();
    QLayoutItem * item = ui->chartLayoutS->takeAt(0);
    if(item->widget())
            delete item->widget();

    QChart* chart = new QChart();
    for(int i = 0; i < allSeries.count(); i++)
    {
        chart->addSeries(allSeries[i]);
    }
    chart->setTitle("Votes");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    categories << "?" << "0" << "1/2" << "1" << "2" << "3" << "5" << "8";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    axisX->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axisX);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayoutS->addWidget(chartView);

//------------------------EVERYTHING ABOVE--------------------

}
