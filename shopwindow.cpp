#include "shopwindow.h"
#include "ui_shopwindow.h"
#include "mainwindow.h"

ShopWindow::ShopWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ShopWindow)
{
    ui->setupUi(this);
}

ShopWindow::~ShopWindow()
{
    delete ui;
}

void ShopWindow::on_pushButton_clicked()
{
    hide();
    //sW->setLogin(loginEdit->text());
    mW = new MainWindow();
    mW->show();
}

