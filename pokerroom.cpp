#include "pokerroom.h"
#include "ui_pokerroom.h"
#include "mainwindow.h"

pokerroom::pokerroom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pokerroom)
{
    ui->setupUi(this);
}

pokerroom::~pokerroom()
{
    delete ui;
}

void pokerroom::on_pushButton_clicked()
{
    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();
}

