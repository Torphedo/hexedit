#pragma once

#include <cstddef>
#include "Buffer.h"

struct Marker {
    size_t pos = 0;
    const Buffer& buf;
    Marker(const Buffer& buf) : buf(buf) {}

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveToOrigin();
    void moveToEnd();
    void jumpUp();
    void jumpDown();

    void displayByte(int colorPair);

    void show();
    void hide();
    std::size_t getPos();
    void setPos(size_t pos);
};