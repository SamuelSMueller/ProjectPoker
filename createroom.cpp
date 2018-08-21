#include "createroom.h"
#include "ui_createroom.h"

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
