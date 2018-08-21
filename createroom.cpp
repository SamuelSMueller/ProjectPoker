#include "createroom.h"
#include "ui_createroom.h"
#include "pokerroom.h"
#include "mainwindow.h"
createroom::createroom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createroom)
{
    ui->setupUi(this);
}

createroom::~createroom()
{
    delete ui;
}

void createroom::on_pushButton_clicked()
{
    QString roomname = ui->lineEdit_2->text();
    qDebug() << roomname;
    close();
    pokerroom * pkRm = new pokerroom;
    pkRm->show();
}

void createroom::on_pushButton_2_clicked()
{
    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();
}
