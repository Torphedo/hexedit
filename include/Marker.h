#pragma once

#include <cstddef>

namespace Marker {
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveToOrigin();
    void moveToEnd();

    void show();
    void hide();
    std::size_t getPos();
}
