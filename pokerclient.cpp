#include "pokerclient.h"
#include "ui_pokerclient.h"
#include "mainwindow.h"

pokerclient::pokerclient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pokerclient)
{
    ui->setupUi(this);

    ui->pushButton_disconnect->setVisible(false);

    client = new ClientStuff("localhost", 6547);

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
    ui->textEdit_log->append(msg);
}

void pokerclient::gotError(QAbstractSocket::SocketError err)
{
    //qDebug() << "got error";
    QString strError = "unknown";
    switch (err)
    {
        case 0:
            strError = "Could not join Room";
            break;
        case 1:
            strError = "Room Admin closed the Room";
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
}

void pokerclient::on_pushButton_send_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << ui->lineEdit_message->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}

void pokerclient::on_pushButton_disconnect_clicked()
{
    client->closeConnection();
}

void pokerclient::on_pushButton_Exit_clicked()
{
    on_pushButton_disconnect_clicked();
    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();
}
