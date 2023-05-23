#include "ghost.h"
#include <QPainter>

Ghost::Ghost(QPoint position, int size)
    : position(position), size(size)
{

}

Ghost::~Ghost()
{

}
QRect Ghost::getBoundingBox() const
{
    return QRect(position.x() - size / 2, position.y() - size / 2, size, size);
}

QPoint Ghost::getPosition() const
{
    return position;
}
