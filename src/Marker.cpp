#include "Marker.h"
#include "Global.h"
#include "Buffer.h"
#include "Table.h"
#include "Color.h"
#include "Base.h"

#include <curses.h>
#include <cstddef>

#include <common/int.h>

void Marker::setPos(size_t pos) {
    pos = MIN(pos, buf.fileSize);
    hide();
    this->pos = pos;
    Table::scrollIntoView(pos);
    show();
}

size_t Marker::getPos() { return pos; }

void Marker::jumpUp()       { setPos(pos - 10 * G::cols); }
void Marker::jumpDown()     { setPos(pos + 10 * G::cols); }
void Marker::moveUp()       { setPos(pos - G::cols); }
void Marker::moveDown()     { setPos(pos + G::cols); }
void Marker::moveLeft()     { setPos(pos - 1); }
void Marker::moveRight()    { setPos(pos + 1); }
void Marker::moveToOrigin() { setPos(0); }
void Marker::moveToEnd() {
    this->pos = buf.fileSize - 1;
}

void Marker::displayByte(int colorPair) {
    int x, y;
    attron(colorPair);

    Table::pos2coords(pos, x, y);
    move(y, x);
    printw(Base::toHex(buf.at(pos)));
    Table::pos2coordsText(pos, x, y);
    move(y, x);
    addch(Base::toText(buf.at(pos)));

    attroff(colorPair);
    refresh();
}

void Marker::show() {
    displayByte(ColorPair::HIGHLIGHT);
}

void Marker::hide() {
    bool modified;
    buf.at(pos, modified);

    displayByte(modified ? ColorPair::MODIFIED : ColorPair::DEFAULT);
}
