#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createroom.h"
#include "joinroom.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("C:/Users/Samuel/Documents/Projects/ProjectPoker/logo.png");
    ui->label_pic->setPixmap(pix.scaledToHeight(ui->label_pic->height()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();
    createroom * crRm = new createroom();
    crRm->setUsername(username);
    crRm->show();
    close();

}

void MainWindow::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    joinroom * jnRm = new joinroom;
    jnRm->setUsername(username);
    jnRm->show();
    close();


}
