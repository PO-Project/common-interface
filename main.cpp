
// Compile with: g++ main.cpp -lncurses -o example
// Run with ./example

#include "ExampleBackend.hpp"
#include "ExampleTool.hpp"

int main(int argc, char *argv[])
{

    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();
    refresh();

    {
        tools::ExampleTool tool;
        backends::ExampleBackend backend;
        backend.setTool(tool);
        tool.setBackend(backend);
        backend();
    }

    endwin();
    return 0;
}