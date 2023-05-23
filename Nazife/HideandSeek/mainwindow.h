#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include "ghost.h"
#include "player.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updatePositions();

private:
    QVector<Player*> players;
    QTimer* timer;
    QVector<Ghost*> ghosts;
};

#endif // MAINWINDOW_H
