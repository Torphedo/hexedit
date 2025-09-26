#include "Marker.h"
#include "Global.h"
#include "Table.h"
#include "Color.h"
#include "Base.h"

#include <curses.h>
#include <cstddef>

#include <common/int.h>

void Marker::setPos(size_t pos) noexcept {
    pos = MIN(pos, buf.fileSize);
    hide();
    this->pos = pos;
    Table::scrollIntoView(pos);
    show();
}

void Marker::move(s16 diff) noexcept {
    setPos(pos + diff);
}
void Marker::moveVertical(s16 diff) noexcept {
    diff = -diff; // Make negative values be visually up and vice versa
    move(diff * G::cols);
}

void Marker::displayByte(int colorPair) const noexcept {
    int x, y;
    attron(colorPair);

    Table::pos2coords(pos, x, y);
    ::move(y, x);
    printw(Base::toHex(buf.at(pos)));
    Table::pos2coordsText(pos, x, y);
    ::move(y, x);
    addch(Base::toAscii(buf.at(pos)));

    attroff(colorPair);
    refresh();
}

void Marker::show() const noexcept {
    displayByte(ColorPair::HIGHLIGHT);
}

void Marker::hide() const noexcept {
    bool modified;
    buf.at(pos, modified);

    displayByte(modified ? ColorPair::MODIFIED : ColorPair::DEFAULT);
}
