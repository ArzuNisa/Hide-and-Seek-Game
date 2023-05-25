#include <QApplication>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTime>

void createGhost() {
    int argc = 0;
    char *argv[] = {};
    QApplication app(argc, argv);


    QGraphicsScene scene;
    QMainWindow window;
    window.setFixedSize(800, 600);


    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.show();


    srand(QTime::currentTime().msec());

    // Create nine circles at random locations
    const int numCircles = 9;
    const qreal circleRadius = 10.0; // in cm

    for (int i = 0; i < numCircles; ++i) {

        qreal x = rand() % 800 - 400;
        qreal y = rand() % 600 - 300;

        
        QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(
            x - circleRadius, y - circleRadius, circleRadius * 2, circleRadius * 2);
        circle->setPen(Qt::NoPen);
        circle->setBrush(Qt::red);  

       
        scene.addItem(circle);
    }

    app.exec();
}


