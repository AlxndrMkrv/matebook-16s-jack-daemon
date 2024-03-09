#pragma once
#include "EventDev.hxx"
#include <memory>

class Poll {
public:
    enum class Result { TIMEOUT, GOT_EVENT, FAILURE };

    Poll(const EventDeviceDescriptor & fd, const int timeout_ms);
    ~Poll();

    Result run();

private:
    struct Data;
    Data * _data;
};
