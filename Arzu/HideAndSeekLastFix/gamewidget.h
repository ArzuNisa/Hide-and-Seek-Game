#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <vector>
#include <QGraphicsPixmapItem>

#define MAX_WİDTH 1920
#define MAX_HEIGHT 1080


struct Player {

    int x, y; // Player's position
    int width, height; // Player's size
    int speed; // Player's speed
    int score; // Player's score
    int whiteSqureSize = height * 3;
    int whiteSlide = whiteSqureSize/2.0;
};

struct Ghost {
    int x, y; // Ghost's position
    int width, height; // Ghost's size
    bool isHidden; // Ghost's visibility
    bool isCatch;
};

class GameWidget : public QWidget {
    int scWidth = MAX_WİDTH;
    int scHeight = MAX_HEIGHT;
    int direction = 0, direction2 = 0;

public:
    GameWidget(QWidget *parent = nullptr);
    virtual ~GameWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resetGame();
    void timerEvent(QTimerEvent *event)override;

private:
    Player player;
    Player player2;
    std::vector<Ghost> ghosts;
    QTimer timer;
    bool isGameFinished;
    int numGhosts = 20;
};

#endif // GAMEWIDGET_H
