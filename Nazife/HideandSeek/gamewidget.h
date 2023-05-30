#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QPainter>
#include <QDebug>
#include <QFont>
#include <QWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QRandomGenerator>

struct Player {
    int x, y; // Oyuncunun konumu
    int width, height; // Oyuncunun boyutu
    int speed; // Oyuncunun hızı
    int score; // Oyuncunun skoru
    int whiteSqureSize = height * 4;
    int whiteSlide = whiteSqureSize / 2.0;
};

struct Ghost {
    int x, y; // Hayaletin konumu
    int width, height; // Hayaletin boyutu
    bool isHidden; // Hayaletin görünürlüğü
    bool isCatch;
};

class GameWidget : public QWidget {
    int scWidth = 1920;
    int scHeight = 1080;

public:
    GameWidget(QWidget *parent = nullptr);
    void checkGhostCollisions();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


private:
    void resetGame();
    void timerEvent(QTimerEvent *event) override;

    Player player;
    Player player2;
    std::vector<Ghost> ghosts;
    QTimer timer;
    bool isGameFinished;
    int numGhosts = 2;
};


#endif // GAMEWIDGET_H
