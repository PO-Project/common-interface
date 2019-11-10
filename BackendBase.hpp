#ifndef BACKENDBASE_H_
#define BACKENDBASE_H_

#include <string>
#include <functional>

namespace tools
{
class ToolBase;
}

namespace backends
{
class BackendBase
{
public:
    virtual void bind(const std::string &command, std::function<void()> callback, const std::string &helpMessage) noexcept = 0;
    virtual void operator()() = 0;
    virtual void setTool(tools::ToolBase &newTool) { tool = &newTool; }

protected:
    tools::ToolBase *tool;
};
} // namespace backends
#endif /* !BACKENDBASE_H_ */
