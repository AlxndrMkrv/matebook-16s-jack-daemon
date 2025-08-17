#pragma once

#include <filesystem>

import Posix;

class JackHandler : public matebook::posix::InputEvent {
public:
    JackHandler(const std::filesystem::path & event_dev,
                const std::filesystem::path & snd_dev);

    void onEvent(const int value) override;
    void onInvalidEvent(const uint16_t type, const uint16_t code,
                        const int value) override;

    void onJackInserted();
    void onJackRemoved();

private:
    matebook::posix::HdaDevice _hdaDev;
};
