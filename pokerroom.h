#ifndef POKERROOM_H
#define POKERROOM_H

#include <QDialog>

namespace Ui {
class pokerroom;
}

class pokerroom : public QDialog
{
    Q_OBJECT

public:
    explicit pokerroom(QWidget *parent = nullptr);
    ~pokerroom();

private slots:
    void on_pushButton_clicked();

private:
    Ui::pokerroom *ui;
};

#endif // POKERROOM_H
