#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createroom.h"
#include "joinroom.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();
    qDebug() << username;
    createroom * crRm = new createroom;
    crRm->show();
    close();

}

void MainWindow::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    qDebug() << username;
    joinroom * jnRm = new joinroom;
    jnRm->show();
    close();


}
