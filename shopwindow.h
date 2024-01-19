#ifndef SHOPWINDOW_H
#define SHOPWINDOW_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class ShopWindow;
}

class ShopWindow : public QWidget
{
    Q_OBJECT

public:
    ShopWindow *sW;
    QString login;

    explicit ShopWindow(QWidget *parent = nullptr);
    ~ShopWindow();

    QString getLogin(){
        return loginEdit->text();
    }

    void setLogin(QString l){
        this->login = l;
    }


private slots:
    void on_pushButton_clicked();

private:
    Ui::ShopWindow *ui;
    MainWindow *mW;

    QLineEdit *loginEdit;

};

#endif // SHOPWINDOW_H
