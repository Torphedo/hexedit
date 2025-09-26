#pragma once
#include <cstddef>
#include <common/int.h>

#include "Buffer.h"

struct Marker {
    size_t pos = 0;
    const Buffer& buf;
    explicit Marker(const Buffer& buf) : buf(buf) {}

    // Positive values are visually up, not "up" as in forward in the buffer
    void moveVertical(s16 diff) noexcept;
    void move(s16 diff) noexcept;

    void displayByte(int colorPair) const noexcept;

    void show() const noexcept;
    void hide() const noexcept;
    void setPos(size_t pos) noexcept;
};