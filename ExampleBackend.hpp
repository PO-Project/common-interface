#ifndef EXAMPLEBACKEND_H_
#define EXAMPLEBACKEND_H_

#include <map>
#include <ncurses.h>
#include "BackendBase.hpp"
#include "ToolBase.hpp"

namespace backends
{
class ExampleBackend : public BackendBase
{
public:
    void bind(const std::string &command, std::function<void()> callback, const std::string &helpMessage) noexcept override
    {
        // This example ignores helpMessage
        callbacks[command] = callback;
    }
    void operator()() override
    {
        int inputCharacter;
        while (true)
        {
            if (tool->getEntry("EXIT") == "true")
                break;
            inputCharacter = getch();
            if (inputCharacter == ':')
            {
                move(0, 0);
                echo();
                std::string command(256, '\0');
                scanw("%s", command.data());
                command.erase(command.find('\0'));
                noecho();
                move(0, 0);
                clrtoeol();
                move(LINES - 1, 0);
                if (callbacks.count(command))
                    callbacks[command]();
                else
                {
                    clrtoeol();
                    printw("Command %s not found.", command.c_str());
                }
                continue;
            }
            if (inputCharacter != KEY_BACKSPACE)
                tool->setEntry("KEY", std::string(1, inputCharacter));
            else
                tool->setEntry("KEY", "<DEL>");

            if (callbacks.count("<EDITION>"))
                callbacks["<EDITION>"]();
        }
    }

private:
    std::map<std::string, std::function<void()>> callbacks;
};
} // namespace backends
#endif /* !EXAMPLEBACKEND_H_ */
