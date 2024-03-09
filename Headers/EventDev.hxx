#pragma once
#include <filesystem>
#include <string_view>

class EventDeviceDescriptor {
public:
    EventDeviceDescriptor(const EventDeviceDescriptor &) = delete;

    explicit EventDeviceDescriptor(
        const std::filesystem::path & event_dev_file);
    ~EventDeviceDescriptor();

    inline bool ready() const { return _fd >= 0; }

    inline const std::string_view error() const { return _error; }

    inline operator int() const { return _fd; }

private:
    int _fd;
    std::string _error;
};
