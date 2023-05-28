#include <QApplication>
#include "gamewidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GameWidget gameWidget;
    gameWidget.show();
    return app.exec();
}
