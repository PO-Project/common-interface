#ifndef EXAMPLETOOL_H_
#define EXAMPLETOOL_H_

#include <map>
#include <ncurses.h>
#include "ToolBase.hpp"
#include "BackendBase.hpp"

namespace tools
{
class ExampleTool : public ToolBase
{
public:
    ExampleTool()
    {
        window = newwin(LINES - 2, COLS, 1, 0);
        box(window, 0, 0);
        wrefresh(window);
        move(LINES - 1, 0);
    }
    ~ExampleTool()
    {
        wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wrefresh(window);
        delwin(window);
    }
    void setEntry(const std::string &key, const std::string &value) noexcept override
    {
        entries[key] = value;
    }
    std::string getEntry(const std::string &key) noexcept override
    {
        return entries[key];
    }
    void setBackend(backends::BackendBase &newBackend) override
    {
        ToolBase::setBackend(newBackend);

        setEntry("EXIT", "false");

        newBackend.bind("<EDITION>", [this]() {
            std::string temp = getEntry("KEY");

            if (temp == "<DEL>")
            {
                if (text.size())
                {
                    text.erase(text.end() - 1);
                }
                temp = text;
            }
            else
            {
                text.insert(text.end(), temp.begin(), temp.end());
                temp = text;
            }

            std::size_t pos = 0;
            while ((pos = temp.find("\n", pos)) != std::string::npos)
            {
                temp.replace(pos, 1, "\n ");
                pos += 2;
            }

            werase(window);
            mvwprintw(window, 1, 1, temp.c_str());
            box(window, 0, 0);
            wrefresh(window);
        },
                        "");

        newBackend.bind("exit", [this]() {
            setEntry("EXIT", "true");
        },
                        "");
    }

private:
    std::map<std::string, std::string> entries;
    WINDOW *window;
    std::string text;
};
} // namespace tools
#endif /* !EXAMPLETOOL_H_ */