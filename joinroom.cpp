#include "joinroom.h"
#include "ui_joinroom.h"
#include "pokerroom.h"
#include "mainwindow.h"

joinroom::joinroom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::joinroom)
{
    ui->setupUi(this);
}

joinroom::~joinroom()
{
    delete ui;
}

void joinroom::on_pushButton_clicked()
{
    //QString roomname = ui->lineEdit_2->text();
    //qDebug() << roomname;
    close();
    pokerroom pkRm;
    pkRm.setModal(true);
    pkRm.exec();
}

void joinroom::on_pushButton_2_clicked()
{
    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();
}
