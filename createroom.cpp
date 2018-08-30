#include "createroom.h"
#include "ui_createroom.h"
#include "pokerserver.h"
#include "mainwindow.h"

createroom::createroom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createroom)
{
    ui->setupUi(this);
    ui->lineEdit_4->setReadOnly(true);
    password = "";
}

createroom::~createroom()
{
    delete ui;
}

void createroom::on_pushButton_clicked()
{
    if(ui->checkBox->isChecked() == true)
    {
        password = ui->lineEdit_4->text();

    }
    close();
    pokerserver * pkRm = new pokerserver();
    pkRm->setUsername(username);
    pkRm->setPassword(password);
    pkRm->show();
}

void createroom::on_pushButton_2_clicked()
{
    MainWindow * mainWindow = new MainWindow;
    mainWindow->show();
    close();
}

void createroom::setUsername(QString uName)
{
    username = uName;
}


void createroom::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0)
        ui->lineEdit_4->setReadOnly(true);
    else
        ui->lineEdit_4->setReadOnly(false);

}
