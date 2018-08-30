#include "pokerclient.h"
#include "ui_pokerclient.h"
#include "mainwindow.h"
#include "info.h"
using namespace QtCharts;


pokerclient::pokerclient(QWidget *parent, QString uName) :
    QDialog(parent),
    ui(new Ui::pokerclient)
{
    username = uName;
    voted = true;
    ui->setupUi(this);
    voteNum = -1;

    QList<QBarSeries *> Qseries;
    Qseries.append(Qseries);

    makechart(Qseries);

    ui->pushButton_disconnect->setVisible(false);

    client = new ClientStuff(netInfo, portInfo, nullptr, username);

    setStatus(client->getStatus());

    connect(client, &ClientStuff::hasReadSome, this, &pokerclient::receivedSomething);
    connect(client, &ClientStuff::statusChanged, this, &pokerclient::setStatus);
    // FIXME change this connection to the new syntax
    connect(client->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(gotError(QAbstractSocket::SocketError)));
}

pokerclient::~pokerclient()
{
    delete client;
    delete ui;
}


void pokerclient::setStatus(bool newStatus)
{
    if(newStatus)
    {
        ui->label_status->setText(
                    tr("<font color=\"green\">In Room</font>"));
        ui->pushButton_connect->setVisible(false);
        ui->pushButton_disconnect->setVisible(true);
    }
    else
    {
        ui->label_status->setText(
                tr("<font color=\"green\">Not In Room</font>"));
        ui->pushButton_connect->setVisible(true);
        ui->pushButton_disconnect->setVisible(false);
    }
}

void pokerclient::receivedSomething(QString msg)
{
    if(msg == "/CLEARLIST/")
    {
        currentUsers.clear();
    }

    else if(msg.startsWith("/ADDLIST/"))
    {
        msg.remove(0,9);
        currentUsers.append(msg);
    }

    else if(msg == "/ENDLIST/")
    {
        ui->listWidget->clear();
        for(int i = 0; i<currentUsers.count(); i++)
        {
            ui->listWidget->addItem(currentUsers.at(i));
        }
    }
    else if(msg == "/ERRNAME/")
    {
        QMessageBox msgBox;
        msgBox.setText("This user name is taken.");
        msgBox.setInformativeText("Please rejoin with a new username.");
        msgBox.exec();
        client->closeConnection();

        MainWindow * mainWindow = new MainWindow;
        mainWindow->show();
        close();
    }
    else if(msg == "/ERRPASS/")
    {
        QMessageBox msgBox;
        msgBox.setText("Your password is incorrect.");
        msgBox.setInformativeText("Please rejoin with the correct password.");
        msgBox.exec();
        client->closeConnection();

        MainWindow * mainWindow = new MainWindow;
        mainWindow->show();
        close();
    }
    else if(msg == "/STVOTE/")
    {
        userVotes.clear();
    }
    else if(msg.startsWith("/VOTEN/"))
    {
        msg.remove(0,7);
        userVotes.append(msg);
    }
    else if(msg == "/RESETVOTE/")
    {
        voted = false;
    }
    else if(msg == "/ENDVOTE/")
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
        //----------------------REWRITE ABOVE----------------------//
    }
    else
    {
        ui->textEdit_log->append(msg);
    }

}

void pokerclient::gotError(QAbstractSocket::SocketError err)
{

    QString strError = "unknown";
    switch (err)
    {
        case 0:
            strError = "Could not join Room";
            break;
        case 1:
            strError = "Admin closed the Room";
            break;
        case 2:
            strError = "Unable to find Room";
            break;
        case 5:
            strError = "Room not open or otherwise unavailable";
            break;
        default:
            strError = "Unknown error";
    }

    ui->textEdit_log->append(strError);
}

void pokerclient::on_pushButton_connect_clicked()
{
    client->connect2host();
    //------------------------------------------------EVERYTHING BELOW
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << ("/USER/"+username+"-/PASS/"+password);

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
    //------------------------------------------------EVERYTHING ABOVE
}

void pokerclient::on_pushButton_send_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << (username + ": " + ui->lineEdit_message->text());

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}

void pokerclient::on_pushButton_disconnect_clicked()
{
    if(voted == false)
    {
        voteNum = -1;
        on_pushButton_Vote_clicked(); //make sure if a user leaves it doesn't break the current vote, sends default -1 vote
    }
    //----------------------------------------------EVERYTHING BELOW
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << ("/DCON/"+username);

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
    //-----------------------------------------------EVERYTHING ABOVE
    client->tcpSocket->waitForBytesWritten();

    ui->listWidget->clear();

    ui->textEdit_log->append("Exited Room.");
    client->closeConnection();
}

void pokerclient::on_pushButton_Exit_clicked()
{
    on_pushButton_disconnect_clicked();
    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();
}



void pokerclient::on_pushButton_Vote8_clicked()
{
    voteNum = "8";
}
void pokerclient::on_pushButton_Vote7_clicked()
{
    voteNum = "5";
}
void pokerclient::on_pushButton_Vote6_clicked()
{
    voteNum = "3";
}
void pokerclient::on_pushButton_Vote5_clicked()
{
    voteNum = "2";
}
void pokerclient::on_pushButton_Vote4_clicked()
{
    voteNum = "1";
}
void pokerclient::on_pushButton_Vote3_clicked()
{
    voteNum = "1/2";
}
void pokerclient::on_pushButton_Vote2_clicked()
{
    voteNum = "0";
}
void pokerclient::on_pushButton_Vote1_clicked()
{
    voteNum = "?";
}

void pokerclient::on_pushButton_Vote_clicked()
{
    if(voted == false)
    {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        //out.setVersion(QDataStream::Qt_5_10);
        out << quint16(0) << ("/VOTEN/" + voteNum + "-/WHOS/" + username);

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        client->tcpSocket->write(arrBlock);
        voted = true;
        ui->textEdit_log->append("You have voted, your vote was " +voteNum+ ".");
    }
    else
        ui->textEdit_log->append("Please wait for the admin to enable voting.");
}


void pokerclient::makechart(QList<QBarSeries *> allSeries)
{
    userVotes.clear();
    QLayoutItem * item = ui->horizontalLayout_5->takeAt(0);
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
    ui->horizontalLayout_5->addWidget(chartView);
}



void pokerclient::setPassword(QString pass)
{
    password = pass;
}
