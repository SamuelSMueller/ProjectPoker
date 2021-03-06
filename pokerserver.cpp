#include "pokerserver.h"
#include "ui_pokerserver.h"
#include "mainwindow.h"
#include "info.h"

pokerserver::pokerserver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pokerserver)
{
    newProgress = true;
    ui->setupUi(this);
    QList<QBarSeries *> Qseries;
    Qseries.append(Qseries);

    makechart(Qseries);

    ui->progressBar->setRange(0,1);
    ui->progressBar->setValue(0);

    ui->progressBar->setStyleSheet("QProgressBar {"
                                       "border: 2px #767676;"
                                       "border-radius: 10px;"
                                       "text-align: center;"
                                       "color: #1cafa5;"
                                   "}"
                                   "QProgressBar::chunk {"
                                       "background-color: #1cafa5;"
                                       "border-radius: 10px;"
                                   "}"
                                   );

    ui->pushButton_Reset->setText("Start Vote");
    voteComplete = false;

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
    ui->textEdit_log->append(QString("<b> Room is opened. </b>"));
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
        ui->textEdit_log->append(QString("<b> Room is closed. </b>"));

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
                    .arg("Room is opened, number of current members:")
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
 // ui->textEdit_log->append("A new player has joined.");
}

void pokerserver::smbDisconnectedFromServer()
{
   // ui->textEdit_log->append("A player has left.");
}

void pokerserver::gotNewMesssage(QString msg, QTcpSocket *clientSocket)
{
    if(msg.startsWith("/USER/"))
    {
        QStringList conString = msg.split('-');
        QString userString = conString[0];
        QString passString = conString[1];

        passString.remove(0,6);
        if(passString != password)
        {
            server->sendToClient(clientSocket, "/ERRPASS/");
        }

        else
        {
            userString.remove(0,6);
            if(currentUsers.isEmpty())
            {
                currentUsers.append("Admin: "+username);
            }

            int j = currentUsers.indexOf(("Player: "+userString));
            QList<QTcpSocket *> clients = server->getClients();

            if(j == -1)
            {
               currentUsers.append("Player: "+userString);
               if(voteComplete == false)
               {
                   int newMax = ui->progressBar->maximum();
                   if(newProgress == true)
                       newProgress = false;
                   else
                   ui->progressBar->setMaximum(++newMax);
               }
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
                int newValue = ui->progressBar->value();
                ui->progressBar->setValue(--newValue);
                int newMax = ui->progressBar->maximum();
                if(newMax > 1)
                    ui->progressBar->setMaximum(--newMax);
                else
                    newProgress = true;
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
        if(voteComplete == false)
        {
            QStringList votingString = msg.split('-');
            QString voteString = votingString[0];
            QString whoString = votingString[1];
            whoString.remove(0,6);
            voteString.remove(0,7);
            ui->textEdit_log->append(QString("<font color=\"green\"><b>%1</b> voted <b>%2</b>.</font>")
                                     .arg(whoString)
                                     .arg(voteString));
            userVotes.append(voteString);
            int newValue = ui->progressBar->value();
            ui->progressBar->setValue(++newValue);

            if(ui->progressBar->value()>=(((currentUsers.count()-1)/2)))
            {
                ui->progressBar->setStyleSheet("QProgressBar {"
                                                   "border: 2px #767676;"
                                                   "border-radius: 10px;"
                                                   "text-align: center;"
                                                   "color: #ffffff;"
                                               "}"
                                               "QProgressBar::chunk {"
                                                   "background-color: #1cafa5;"
                                                   "border-radius: 10px;"
                                               "}"
                                               );
            }


            if(userVotes.count() == (currentUsers.count()-1))
            {

                QList<QTcpSocket *> clients = server->getClients();
                for(int i = 0; i < clients.count(); i++)
                    server->sendToClient(clients.at(i), "/STVOTE/");

                for(int j = 0; j < userVotes.count(); j++)
                {
                    QString sndVote = ("/VOTEN/" + userVotes.at(j));
                    for(int i = 0; i < clients.count(); i++)
                        server->sendToClient(clients.at(i), sndVote);
                }

                //------WRITTEN TERRIBLY, LATE AT NIGHT, AFTER HOMEWORK. WILL BE REWRITTEN LATER--------//
                int first = 0;
                int second = 0;
                int third = 0;
                int fourth = 0;
                int fifth = 0;
                int sixth = 0;
                int seventh = 0;
                int eigth = 0;

                QString color1 = "#1cafa5";
                QString color2 = "#196f64";
                QString color3 = "#767676";
                QList<QString> colors;
                colors.append(color1);
                colors.append(color2);
                colors.append(color3);

                QTime time = QTime::currentTime();
                QRandomGenerator rand((uint)time.msec());

                QBarSet *set0 = new QBarSet("?");
                int gen = rand.bounded(0, 2);
                set0->setColor(colors[gen]);

                QBarSet *set1 = new QBarSet("0");
                set1->setColor(colors[(++gen%3)]);

                QBarSet *set2 = new QBarSet("1/2");
                set2->setColor(colors[(++gen%3)]);

                QBarSet *set3 = new QBarSet("1");
                set3->setColor(colors[(++gen%3)]);

                QBarSet *set4 = new QBarSet("2");
                set4->setColor(colors[(++gen%3)]);

                QBarSet *set5 = new QBarSet("3");
                set5->setColor(colors[(++gen%3)]);

                QBarSet *set6 = new QBarSet("5");
                set6->setColor(colors[(++gen%3)]);

                QBarSet *set7 = new QBarSet("8");
                set7->setColor(colors[(++gen%3)]);

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
                series0->setLabelsVisible(true);

                allSeries.append(series0);
                series1->append(set1);
                series1->setLabelsVisible(true);

                allSeries.append(series1);
                series2->append(set2);
                series2->setLabelsVisible(true);

                allSeries.append(series2);
                series3->append(set3);
                series3->setLabelsVisible(true);

                allSeries.append(series3);
                series4->append(set4);
                series4->setLabelsVisible(true);

                allSeries.append(series4);
                series5->append(set5);
                series5->setLabelsVisible(true);

                allSeries.append(series5);
                series6->append(set6);
                series6->setLabelsVisible(true);

                allSeries.append(series6);
                series7->append(set7);
                series7->setLabelsVisible(true);

                allSeries.append(series7);

                makechart(allSeries);

                for(int i = 0; i < clients.count(); i++)
                    server->sendToClient(clients.at(i), "/ENDVOTE/");
                //----------------------REWRITE ABOVE----------------------//
            }
        } //end if(voteComplete == false)

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

void pokerserver::setPassword(QString pass)
{
    password = pass;
}

void pokerserver::on_pushButton_Shuffle_clicked()
{
    QTime time = QTime::currentTime();

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
    QValueAxis *axisY = new QValueAxis();

    axisY->setMinorTickCount(-1);
    axisY->setLabelFormat("%i");
    axisX->append(categories);

    chart->createDefaultAxes();

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);
    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayoutS->addWidget(chartView);
    voteComplete = true;
}

void pokerserver::on_pushButton_Reset_clicked()
{
    ui->pushButton_Reset->setText("Reset Vote");

    voteComplete = false;
    userVotes.clear();
    QList<QTcpSocket *> clients = server->getClients();
    for(int i = 0; i < clients.count(); i++)
        server->sendToClient(clients.at(i), "/RESETVOTE/");
    ui->progressBar->setRange(0,clients.count());
    ui->progressBar->setValue(0);
    ui->progressBar->setStyleSheet("QProgressBar {"
                                       "border: 2px #767676;"
                                       "border-radius: 10px;"
                                       "text-align: center;"
                                       "color: #1cafa5;"
                                   "}"
                                   "QProgressBar::chunk {"
                                       "background-color: #1cafa5;"
                                       "border-radius: 10px;"
                                   "}"
                                   );


}
