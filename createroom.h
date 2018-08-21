#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QDialog>

namespace Ui {
class createroom;
}

class createroom : public QDialog
{
    Q_OBJECT

public:
    explicit createroom(QWidget *parent = nullptr);
    ~createroom();

private:
    Ui::createroom *ui;
};

#endif // CREATEROOM_H
