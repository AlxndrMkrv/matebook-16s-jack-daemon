#pragma once

#include "Poll.hxx"
#include <poll.h>

struct Poll::Data {
    const EventDeviceDescriptor & _fd;
    pollfd _pfd;
    const int _timeout;
};
