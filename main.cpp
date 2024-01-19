#include "shopwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ShopWindow window;
    window.show();
    return app.exec();
}
