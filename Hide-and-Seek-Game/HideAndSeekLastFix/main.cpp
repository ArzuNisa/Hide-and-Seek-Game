#include "mediaplayer.h"
#include "menu.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MediaPlayer mediaPlayer;
    Menu menu;
    menu.show();
    return app.exec();
}
