#ifndef JOINROOM_H
#define JOINROOM_H

#include <QDialog>

namespace Ui {
class joinroom;
}

class joinroom : public QDialog
{
    Q_OBJECT

public:
    explicit joinroom(QWidget *parent = nullptr);
    ~joinroom();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::joinroom *ui;
};

#endif // JOINROOM_H
