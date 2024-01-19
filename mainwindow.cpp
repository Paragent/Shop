#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QMessageBox>
#include <QHeaderView>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QStringList() << "Name" << "Quantity" << "Price" << "Order");

    button = new QPushButton("Order", this);

    QVBoxLayout *layer = new QVBoxLayout;
    layer->addWidget(table);
    layer->addWidget(button);
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layer);
    setCentralWidget(centralWidget);

    connect(button, &QPushButton::clicked, this, &MainWindow::click);


    QFile chequeFile("C:/Qt/Projects/Shop/cheque.txt");
    if (chequeFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        chequeFile.close();
    } else {
        return;
    }
    load();
}



void MainWindow::load() {
    QFile file("C:/Qt/Projects/Shop/products.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file);
    table->setRowCount(0);
    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" - ");
        if (parts.size() == 2) {
            QStringList secondPart = parts[1].split(" | ");
            if (secondPart.size() == 2) {
                table->insertRow(row);
                table->setItem(row, 0, new QTableWidgetItem(parts[0].trimmed()));
                int quantity = secondPart[0].toInt();
                table->setItem(row, 1, new QTableWidgetItem(QString::number(quantity)));
                table->setItem(row, 2, new QTableWidgetItem(secondPart[1].trimmed()));
                QSpinBox *spinBox = new QSpinBox(this);
                spinBox->setMinimum(0);
                spinBox->setMaximum(quantity);
                spin_boxes.append(spinBox);
                table->setCellWidget(row, 3, spinBox);
                row++;
            }
        }
    }
    file.close();
}

void MainWindow::click() {
    struct OrderInfo {
        QString name;
        int quant;
        int price;
    };
    QList<OrderInfo> orders;

    for (int row = 0; row < table->rowCount(); ++row) {
        QString name = table->item(row, 0)->text();
        int quantityAvailable = table->item(row, 1)->text().toInt();
        int price = table->item(row, 2)->text().toInt();
        int quant = spin_boxes[row]->value();

        if (quant > 0 && quant <= quantityAvailable) {
            quantityAvailable -= quant;
            table->item(row, 1)->setText(QString::number(quantityAvailable));
            orders.append({name, quant, price});
        }
    }

    QFile file("C:/Qt/Projects/Shop/products.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    for (int row = 0; row < table->rowCount(); ++row) {
        QString name = table->item(row, 0)->text();
        QString quantity = table->item(row, 1)->text();
        QString price = table->item(row, 2)->text();
        out << name << " - " << quantity << " | " << price << "\n";
    }
    file.close();

    QFile chequeFile("C:/Qt/Projects/Shop/cheque.txt");
    if (!chequeFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }
    QTextStream chequeOut(&chequeFile);

    int totalSum = 0;
    for (const OrderInfo &order : orders) {
        int sum = order.quant * order.price;
        totalSum += sum;
        chequeOut << order.name << ": " << order.quant <<" pcs. - " <<  sum << " $." << "\n";
    }
    chequeOut << "Total: " << totalSum << " $." << "\n\n";
    chequeFile.close();

    QMessageBox::information(this, "Order", "Your order has been placed");
    load();
}
