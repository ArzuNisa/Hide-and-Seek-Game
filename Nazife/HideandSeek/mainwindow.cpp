#include "mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(800, 600); // Pencerenin boyutunu sabitle

    // Oyuncuları oluşturun
    Player* player1 = new Player(this);
    player1->setPosition(QPoint(100, 100));

    Player* player2 = new Player(this);
    player2->setPosition(QPoint(700, 500));

    players.append(player1);
    players.append(player2);

    // Ghostları oluşturun
    Ghost* ghost1 = new Ghost(QPoint(200, 200), 20);
    Ghost* ghost2 = new Ghost(QPoint(500, 300), 20);
    Ghost* ghost3 = new Ghost(QPoint(300, 400), 20);

    ghosts.append(ghost1);
    ghosts.append(ghost2);
    ghosts.append(ghost3);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePositions);
    timer->start(100); // Her 100 ms'de bir pozisyonları güncelle
}


MainWindow::~MainWindow()
{
    for (Player* player : players)
    {
        delete player;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Oyuncuları çiz
    for (Player* player : players)
    {
        QPoint position = player->getPosition();
        int size = 50; // Karelerin boyutunu belirleyin

        QRect playerRect(position.x() - size / 2, position.y() - size / 2, size, size);
        painter.fillRect(playerRect, Qt::blue);
    }

    // Ghostları çiz
    for (Ghost* ghost : ghosts)
    {
        QPoint position = ghost->getPosition();
        int size = 20; // Karelerin boyutunu belirleyin

        QRect ghostRect(position.x() - size / 2, position.y() - size / 2, size, size);
        painter.fillRect(ghostRect, Qt::red);
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);

    for (Player* player : players)
    {
        switch (event->key())
        {
        // Player 1
        case Qt::Key_Left:
            player->moveLeft();
            break;
        case Qt::Key_Right:
            player->moveRight();
            break;
        case Qt::Key_Up:
            player->moveUp();
            break;
        case Qt::Key_Down:
            player->moveDown();
            break;

            // Player 2
        case Qt::Key_A:
            player->moveLeft();
            break;
        case Qt::Key_D:
            player->moveRight();
            break;
        case Qt::Key_W:
            player->moveUp();
            break;
        case Qt::Key_S:
            player->moveDown();
            break;
        }
    }

    update();
}

void MainWindow::updatePositions()
{
    /* for (Player* player : players)
    {
        // Pozisyonları rastgele olarak güncelleme
        int dx = (rand() % 3 - 1) * 10;
        int dy = (rand() % 3 - 1) * 10;

        QPoint newPosition = player->getPosition() + QPoint(dx, dy);
        player->setPosition(newPosition);
    }
*/
    update();
}
