#include "joinroom.h"
#include "ui_joinroom.h"
#include "pokerclient.h"
#include "mainwindow.h"

joinroom::joinroom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::joinroom)
{
    ui->setupUi(this);
    ui->lineEdit_4->setReadOnly(true);
    password = "";
}

joinroom::~joinroom()
{
    delete ui;
}

void joinroom::on_pushButton_clicked()
{
    if(ui->checkBox->isChecked() == true)
    {
        password = ui->lineEdit_4->text();

    }

    close();
    pokerclient * pkCl = new pokerclient(nullptr, username);
    pkCl->setPassword(password);
    pkCl->show();
}

void joinroom::on_pushButton_2_clicked()
{

    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();

}


void joinroom::setUsername(QString uName)
{
    username = uName;
}


void joinroom::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0)
        ui->lineEdit_4->setReadOnly(true);
    else
        ui->lineEdit_4->setReadOnly(false);

}
