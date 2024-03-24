#pragma once

#include <filesystem>

import Posix;

class JackHandler : public InputEvent {
public:
    JackHandler(const std::filesystem::path & event_dev,
                const std::filesystem::path & snd_dev);

    inline void onEvent(int value) override
    {
        value ? onJackInserted() : onJackRemoved();
    }

    void onJackInserted();
    void onJackRemoved();

private:
    HdaDevice _hdaDev;
};
