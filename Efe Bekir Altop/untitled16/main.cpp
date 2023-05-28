#include <QApplication>
#include "menu.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Menu menu;
    menu.show();
    return app.exec();
}
