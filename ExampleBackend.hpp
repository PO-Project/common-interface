#ifndef EXAMPLEBACKEND_H_
#define EXAMPLEBACKEND_H_

#include <map>
#include <ncurses.h>
#include <panel.h>
#include "BackendBase.hpp"
#include "ToolBase.hpp"

namespace backends
{
class ExampleBackend : public BackendBase
{
public:
    ExampleBackend()
    {
        // initialize ncurses
        initscr();
        cbreak();
        keypad(stdscr, true);
        noecho();

        set_escdealy(25);

        bind("quit", [this](){ tryToQuit = true; }, "help we ignore in example");
        bind("donothing", [this](){}, "nothing");
    }
    ~ExampleBackend()
    {
        // end curses session
        endwin();
    }

    void bind(const std::string &command, std::function<void()> callback, const std::string &helpMessage) noexcept override
    {
        // This example ignores helpMessage
        callbacks[command] = callback;
    }
    void operator()() override
    {
        // read input and run binds in a loop

        int inputCharacter;
        while (true)
        {
            // if terminal was resized resize windows
            if (LINES != oldLINES || COLS != oldCOLS)
            {
                ResizeWindow();
                oldLINES = LINES;
                oldCOLS = COLS;
            }

            update_panels();
            doupdate();

            inputCharacter = getch();
            if (inputCharacter == ':')
            {
                // put panel on top of stack
                top_panel(statusPanel);
                echo();
                wmove(statusWindow, 1, 0);
                wclrtoeol(statusWindow);
                std::string command(256, '\0');
                wscanw(statusWindow, "%s", command.data());
                command.erase(command.find('\0'));
                noecho();
                // move panel back to bottom
                bottom_panel(statusPanel);

                if (callbacks.count(command))
                    callbacks[command]();
                else
                    setStatus("Command " + command + " not found");
            }
            else
            {
                if (inputCharacter != KEY_BACKSPACE)
                    tool->setEntry("KEY", std::string(1, inputCharacter));
                else
                    tool->setEntry("KEY", "<DEL>");

                if (callbacks.count("<EDITION>"))
                    callbacks["<EDITION>"]();
            }

            if (tryToQuit)
            {
                if (tool->getEntry("IS_SAVED") == "NO")
                {
                    setStatus("Can't quit, file not saved!");
                    tryToQuit = false;
                }
                else
                    break;
            }
        }
    }

private:

    std::map<std::string, std::function<void()>> callbacks;

    WINDOW *statusWindow = nullptr;
    PANEL *statusPanel = nullptr;

    std::string status;

    int oldLINES = -1, oldCOLS = -1;

    bool tryToQuit = false;

    void ResizeWindow()
    {
        if (statusPanel != nullptr)
        {
            del_panel(statusPanel);
            delwin(statusWindow);
        }
        statusWindow = newwin(2, COLS, LINES-2, 0);
        statusPanel = new_panel(statusWindow);
        bottom_panel(statusPanel);

        wmove(statusWindow, 0, 0);
        whline(statusWindow, '-', COLS);
        displayStatus();

        // tell tool to resize it's windows
        tool->setCoordinates(LINES-2, COLS, 0, 0);
    }
    void setStatus(const std::string &s)
    {
        status = s;
        displayStatus();
    }
    void displayStatus()
    {
        mvwprintw(statusWindow, 1, 0, status.c_str());
        wclrtoeol(statusWindow);
    }
};
} // namespace backends
#endif /* !EXAMPLEBACKEND_H_ */
