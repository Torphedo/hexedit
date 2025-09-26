#include "ArgParser/ArgParser.h"
#include "ArgParser/ArgParseException.h"
#include "Buffer.h"
#include "Global.h"
#include "Marker.h"
#include "Color.h"
#include "Table.h"
#include "Mode.h"

#include <curses.h>
#include <iostream>
#include <cstdio>
#include <string>

const char* HELP_MSG = R"(Hexedit version 1.0
A TUI hex editor

Usage: hexedit [OPTIONS] <file>
Options:
  -h, --help         Print this help message
  -s, --std-colors   Use standard colors, instead of default terminal colors
                     Enable this if there are issues with default colors.
  -c <cols>,         Set the number of columns. Recommended: 4, 8, 16, 32.
    --columns=<cols> There are no restrictions on this number, but columns
                     wider than screen can misbehave.

Note: If you're having color issues (e.g. when your terminal uses light mode),
      use option '-s' or '--std-colors'
)";

const char* SHORT_HELP_MSG = R"(No file provided.
Usage: hexedit [OPTIONS] <file>
Use option '-h' or --help' to get complete help.
)";

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("%s\n", SHORT_HELP_MSG);
        return EXIT_FAILURE;
    }
    
    ArgParser ap(argc, argv, "hs", "c");
    try {
        ap.parse();
    } catch (const ArgParseException &e) {
        printf("Argument parsing error: %s\n", e.what());
        return 2;
    }

    if (ap.isSwitchSet("h") || ap.isSwitchSet("help")) {
        printf("%s\n", HELP_MSG);
        return EXIT_SUCCESS;
    }

    const std::vector<std::string> operands = ap.getOperands();
    if (operands.size() == 0) {
        printf("%s\n", SHORT_HELP_MSG);
        return EXIT_FAILURE;
    }

    const std::string fileName = operands[0];
    const bool stdColors = ap.isSwitchSet("s") || ap.isSwitchSet("std-colors");

    int columns = 16;
    if (ap.isDataOptSet("c")) {
        columns = std::stoi(ap.getDataForOpt("c"));
    }
    if (ap.isDataOptSet("columns")) {
        columns = std::stoi(ap.getDataForOpt("columns"));
    }
    G::cols = columns;
    
    initscr();
    G::height = LINES;
    G::width = COLS;
    curs_set(0);
    cbreak();
    noecho();
    Colors::init(stdColors);
    keypad(stdscr, TRUE);

    Buffer::load(fileName);
    Table::show();
    Marker::show();
    G::setStatusBarText("File Ready");
    Mode::normal();
    
    endwin();
    Buffer::finish();
    return EXIT_SUCCESS;
}
