#ifndef TOOLBASE_H_
#define TOOLBASE_H_

#include <string>

namespace backends
{
class BackendBase;
}

namespace tools
{
class ToolBase
{
public:
    virtual void setEntry(const std::string &key, const std::string &value) noexcept = 0;
    virtual std::string getEntry(const std::string &key) noexcept = 0;
    virtual void setBackend(backends::BackendBase &newBackend) { backend = &newBackend; }

protected:
    backends::BackendBase *backend = nullptr;
};
} // namespace tools
#endif /* !TOOLBASE_H_ */