#pragma once

#include <curses.h>
#include <string>

#include "Buffer.h"
#include "Marker.h"

namespace G {
    const int isz = 8;

    extern int height, width;
    extern int cols;
    extern Buffer buf;
    extern Marker mark;

    void setStatusBarText(std::string text);
}

inline void moveRight(int n = 1) {
    move(getcury(stdscr), getcurx(stdscr) + n);
}

inline void moveLeft(int n = 1) {
    move(getcury(stdscr), getcurx(stdscr) - n);
}

inline int printw(std::string str) {
    return printw(str.c_str());
}
