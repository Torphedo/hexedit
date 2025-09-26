#include "Mode.h"
#include "InputSeq.h"
#include "Buffer.h"
#include "Global.h"
#include "Table.h"
#include "Base.h"
#include <common/int.h>

#include <curses.h>
#include <string>
#include <cstddef>

// These values can be found by (in Vim insert mode) doing Ctrl-V, then
// pressing the key (e.g. Ctrl-D), putting that in quotations and assigning it
// to a char. Then you can use your LSP or print to get the value.
enum special_keys {
    CONTROL_D = 4,
    CONTROL_U = 21,
};

void updateModeNormal(Buffer& buf) {
    bool modified = false;
    size_t mPos = 0;
    std::string ogValMsg;
    while (true) {
        mPos = G::mark.pos;
        buf.at(mPos, modified);
        ogValMsg = "";
        if (modified) {
            ogValMsg = "-- (Modified) Original Value: " +
            std::to_string(buf.ogVal(mPos)) +
            " (0x" + Base::toHex(buf.ogVal(mPos)) + ")";
        }

        G::setStatusBarText(
            "Position: " + std::to_string(mPos) +
            " (0x" + Base::toHex(mPos) + ")" +
            " " + ogValMsg
        );

        const int key = getch();
        switch (key) {
        case 'q':
            if (InputSeq::quit()) {
                return;
            }
            break;

        case 'u':
            InputSeq::undo();
            break;

        case 'r':
            InputSeq::revert();
            break;

        case 's':
            InputSeq::save();
            break;

        case CONTROL_U:
            G::mark.moveVertical(10);
            break;
        case CONTROL_D:
            G::mark.moveVertical(-10);
            break;
        case 'g': // Not really correct, but proper logic for 'gg' is annoying
            G::mark.setPos(0);
            break;
        case 'G':
            G::mark.setPos(buf.fileSize - 1);
            break;

        case KEY_UP: case 'k':
            G::mark.moveVertical(1);
            break;

        case KEY_DOWN: case 'j':
            G::mark.moveVertical(-1);
            break;

        case KEY_LEFT: case 'h':
            G::mark.move(-1);
            break;

        case KEY_RIGHT: case 'l':
            G::mark.move(1);
            break;

        case '\n': case 'i':
            updateModeEdit(buf);
            break;
        }
    }
}

void updateModeEdit(Buffer& buf) {
    const std::size_t mPos = G::mark.pos;
    G::setStatusBarText(
        "Editing byte at: " + std::to_string(mPos) + " (0x" + Base::toHex(mPos) + ")" +
        " -- Original value = " +
        std::to_string(static_cast<unsigned char>(buf.at(mPos))) +
        " (0x" + Base::toHex(buf.at(mPos)) + ")"
    );

    G::mark.hide();
    int x, y;
    Table::pos2coords(mPos, x, y);
    move(y, x);
    curs_set(2);
    bool curLeft = true;

    char newHexByte[3];
    innstr(newHexByte, 2);

    while (true) {
        const int key = getch();
        switch (key) {
        case '\n':
            buf.set(mPos, Base::toText(newHexByte));
            fallthrough;
        case 'q':
            Table::refresh();
            curs_set(0);
            G::mark.show();
            return;

        case KEY_LEFT:
        case KEY_RIGHT:
            curLeft ? moveRight() : moveLeft();
            curLeft = !curLeft;
            break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9': case 'a': case 'b':
        case 'c': case 'd': case 'e': case 'f':
            addch(key);
            newHexByte[curLeft] = key;
            if (!curLeft) {
                moveLeft(2);
            }
            curLeft = !curLeft;
            break;
        }
    }
}
