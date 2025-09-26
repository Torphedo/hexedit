#include "InputSeq.h"
#include "Buffer.h"
#include "Global.h"
#include "Table.h"
#include "Marker.h"

#include <curses.h>

namespace InputSeq {
    void save() {
        G::setStatusBarText(
            "-- (s)-Save sequence -- [s=same file, n=new file]");
        const int key = getch();
        switch (key) {
        case 's':
            G::buf.save(SaveOption::SAME_FILE);
            break;
        case 'n':
            G::buf.save(SaveOption::NEW_FILE);
            break;
        }
    }

    bool quit() {
        if (!G::buf.isModified()) {
            return true;
        }

        G::setStatusBarText(
            "-- (q)-Quit sequence - There are unsaved changes -- [q=quit without saving]");

        const int key = getch();
        return key == 'q';
    }

    void revert() {
        if (G::buf.isModified()) {
            G::setStatusBarText(
                "-- (r)-Revert sequence -- [r=undo all changes]");
        } else {
            G::setStatusBarText(
                "-- (r)-Revert sequence -- (No unsaved changes)");
        }

        const int key = getch();
        if (key == 'r' && G::buf.isModified()) {
            G::buf.revert();
            Table::refresh();
            G::mark.show();
        }
    }

    void undo() {
        G::buf.undo(G::mark.getPos());
        Table::refresh();
        G::mark.show();
    }
}
