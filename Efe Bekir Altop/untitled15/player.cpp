#include "player.h"

Player::Player(QObject *parent)
    : QObject(parent)
{

}

void Player::setPosition(const QPoint& newPosition)
{
    position = newPosition;
    emit positionChanged(newPosition);
}

QPoint Player::getPosition() const
{
    return position;
}

void Player::moveLeft()
{
    position.setX(position.x() - 10);
    emit positionChanged(position);
}

void Player::moveRight()
{
    position.setX(position.x() + 10);
    emit positionChanged(position);
}

void Player::moveUp()
{
    position.setY(position.y() - 10);
    emit positionChanged(position);
}

void Player::moveDown()
{
    position.setY(position.y() + 10);
    emit positionChanged(position);
}
