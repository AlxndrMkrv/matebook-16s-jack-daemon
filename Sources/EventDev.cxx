#include "EventDev.hxx"
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace fs = std::filesystem;

EventDeviceDescriptor::EventDeviceDescriptor(
    const std::filesystem::path & event_dev) :
    _fd(open(event_dev.string().c_str(), O_RDONLY))
{
    if (_fd < 0) {
        _error = std::strerror(errno);
    }
}

EventDeviceDescriptor::~EventDeviceDescriptor()
{
    if (_fd > 0)
        close(_fd);
}
