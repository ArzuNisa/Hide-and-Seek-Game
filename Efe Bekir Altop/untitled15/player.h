#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPoint>

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = nullptr);

    void setPosition(const QPoint& position);
    QPoint getPosition() const;
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

signals:
    void positionChanged(const QPoint& newPosition);

private:
    QPoint position;
};

#endif // PLAYER_H
