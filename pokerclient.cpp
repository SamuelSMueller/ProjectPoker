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
    ui->setupUi(this);


    QBarSet *set0 = new QBarSet("1");
    QBarSet *set1 = new QBarSet("2");
    QBarSet *set2 = new QBarSet("3");
    QBarSet *set3 = new QBarSet("4");
    QBarSet *set4 = new QBarSet("5");

    *set0 << 1 << 1 << 2;
    *set1 << 3 << 1 << 2;
    *set2 << 4 << 0 << 2;
    *set3 << 1 << 3 << 2;
    *set4 << 2 << 2 << 2;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);


    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Votes");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    categories << "First" << "Second" << "Third" << "Fourth" << "Fifth";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignCenter);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout_5->addWidget(chartView);


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
    out << quint16(0) << ("/USER/"+username);

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


void pokerclient::setRoomname(QString rName)
{
    roomname = rName;
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
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << ("/VOTEN/" + voteNum);

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}
