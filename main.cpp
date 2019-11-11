
// Compile with: g++ main.cpp -lncurses -o example
// Run with ./example

#include "ExampleBackend.hpp"
#include "ExampleTool.hpp"

int main(int argc, char *argv[])
{
    tools::ExampleTool tool;
    backends::ExampleBackend backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
}
