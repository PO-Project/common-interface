#ifndef EXAMPLETOOL_H_
#define EXAMPLETOOL_H_

#include <map>
#include <ncurses.h>
#include <panel.h>
#include "ToolBase.hpp"
#include "BackendBase.hpp"


// namespace to made the code prettier (and prevent conflicts) 
namespace tools
{
class ExampleTool : public ToolBase
{
public:
    ExampleTool()
    {
        // this example just doesn't allow to quit
        // because "its never saved"
        // (see line 111 in ExampleBackend.hpp)
        setEntry("IS_SAVED", "NO");
    }
    ~ExampleTool()
    {
        if (window != nullptr)
        {
            // release ncurses recourses
            del_panel(panel);
            delwin(window);
        }
    }
    void setEntry(const std::string &key, const std::string &value) noexcept override
    {
        // see declaration of:
        // std::map<std::string, std::string> entries
        entries[key] = value;
    }
    std::string getEntry(const std::string &key) noexcept override
    {
        // see declaration of:
        // std::map<std::string, std::string> entries
        return entries[key];
    }
    void setCoordinates(int width, int height, int startx, int starty) noexcept override
    {
        // recreate window with appropria size
        if (window != nullptr)
        {
            del_panel(panel);
            delwin(window);
        }

        window = newwin(width, height, startx, starty);
        panel = new_panel(window);

        // redraw window's contents
        printText();
    }
    void setBackend(backends::BackendBase &newBackend) override
    {
        // register backend
        // see ToolBase.hpp
        ToolBase::setBackend(newBackend);

        // bind commands to backend
        // it is very important:
        // you should register all your commands here

        newBackend.bind("<EDITION>", [this]() {
            // "<EDITION>" command is special command
            // it is called in "edition mode"
            // (some of backends have such mode,
            // some are in this mode all the time)

            // it is called when keypress event occurs
            // key code is accessible through "KEY" entry
            std::string temp = getEntry("KEY");


            // this example just modify "text":
            if (temp == "<DEL>")
            {
                if (!text.empty())
                    text.pop_back();
            }
            else
                text += temp;

            // redraw the window
            printText();
        },
                        "");

        newBackend.bind("clear", [this]() {
            text.clear();
            printText();
        },
                        "");
    }

private:
    // dictionary maping from string to string
    // general concept:
    // https://en.wikipedia.org/wiki/Associative_array
    // cpp implementaion:
    // https://en.cppreference.com/w/cpp/container/map
    std::map<std::string, std::string> entries;

    // ncurses window and panel
    WINDOW *window = nullptr;
    PANEL *panel = nullptr;

    std::string text;

    void printText()
    {
        // clear window and redraw it
        werase(window);
        mvwprintw(window, 0, 0, text.c_str());
    }
};
} // namespace tools
#endif /* !EXAMPLETOOL_H_ */
