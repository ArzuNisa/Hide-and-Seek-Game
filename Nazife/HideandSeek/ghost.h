#ifndef GHOST_H
#define GHOST_H

#include <QPoint>
#include <QRect>


class Ghost
{
public:
    Ghost(QPoint position, int size);
    ~Ghost();

    QPoint getPosition() const;
    QRect getBoundingBox() const;

private:
    QPoint position;
    int size;
};

#endif // GHOST_H
