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
    createroom crRm;
    crRm.setModal(true);
    crRm.exec();

}

void MainWindow::on_pushButton_2_clicked()
{
    joinroom jnRm;
    jnRm.setModal(true);
    jnRm.exec();
}
