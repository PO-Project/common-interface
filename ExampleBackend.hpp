#ifndef EXAMPLEBACKEND_H_
#define EXAMPLEBACKEND_H_

#include <map>
#include <ncurses.h>
#include <panel.h>
#include "BackendBase.hpp"
#include "ToolBase.hpp"


// namespace to made the code prettier (and prevent conflicts) 
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

        set_escdelay(25);

        // bind its own commands (you can bind some kind of "help" command here)
        // note: capturing this to lambda is good trick
        // https://en.cppreference.com/w/cpp/language/lambda
        bind("quit", [this](){ tryToQuit = true; }, "help we ignore in example");
        bind("donothing", [this](){}, "nothing");
    }
    ~ExampleBackend()
    {
        // end ncurses session
        endwin();
    }

    void bind(const std::string &command, std::function<void()> callback, const std::string &helpMessage) noexcept override
    {
        // this example ignores helpMessage
        // you mustn't ignores it
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
                resizeWindow();
                oldLINES = LINES;
                oldCOLS = COLS;
            }
            
            // redraw (update) window
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
                // read command
                wscanw(statusWindow, "%s", command.data());
                command.erase(command.find('\0'));
                noecho();
                // move panel back to bottom
                bottom_panel(statusPanel);

                // if command for callback exists call it
                if (callbacks.count(command))
                    callbacks[command]();
                else
                    setStatus("Command " + command + " not found");
            }
            else
            {
                // handle special characters
                if (inputCharacter != KEY_BACKSPACE)
                    tool->setEntry("KEY", std::string(1, inputCharacter));
                else
                    tool->setEntry("KEY", "<DEL>");

                // currently proposed special key bindings are:
                /*
                27              "<ESC>"
                KEY_ENTER       "<ENTER>"
                KEY_BACKSPACE   "<DEL>"
                KEY_UP          "<UARROW>"
                KEY_DOWN        "<DARROW>"
                KEY_LEFT        "<LARROW>"
                KEY_RIGHT       "<RARROW>"
                */

                // call "<EDITION>" callback to inform the tool
                // about keypress event
                if (callbacks.count("<EDITION>"))
                    callbacks["<EDITION>"]();
            }

            // tryToQuit is modified in line 28 in "quit" callback
            if (tryToQuit)
            {
                // if project isn't save you mustn't exit
                // (actually, this example won't save anything
                // so you cannot exit this example in this way)
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

    // dictionary maping from string to function
    // general concept:
    // https://en.wikipedia.org/wiki/Associative_array
    // cpp implementaion:
    // https://en.cppreference.com/w/cpp/container/map
    std::map<std::string, std::function<void()>> callbacks;

    // ncurses window and panel
    WINDOW *statusWindow = nullptr;
    PANEL *statusPanel = nullptr;

    // message displaying below window
    std::string status;

    // old size of window
    int oldLINES = -1, oldCOLS = -1;

    bool tryToQuit = false;

    void resizeWindow()
    {   
        // recreate window with appropria size
        if (statusPanel != nullptr)
        {
            del_panel(statusPanel);
            delwin(statusWindow);
        }
        statusWindow = newwin(2, COLS, LINES-2, 0);
        statusPanel = new_panel(statusWindow);
        bottom_panel(statusPanel);

        // redraw status line
        wmove(statusWindow, 0, 0);
        whline(statusWindow, '-', COLS);
        displayStatus();

        // tell tool to resize its windows
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
