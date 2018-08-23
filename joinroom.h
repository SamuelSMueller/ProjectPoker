#ifndef JOINROOM_H
#define JOINROOM_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class joinroom;
}

class joinroom : public QDialog
{
    Q_OBJECT

public:
    explicit joinroom(QWidget *parent = nullptr);
    ~joinroom();
    void setUsername(QString uName);

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    QString username;
    Ui::joinroom *ui;
};

#endif // JOINROOM_H
