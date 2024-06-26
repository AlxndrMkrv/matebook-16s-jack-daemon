#include "JackHandler.hxx"
#include "Globals.hxx"
#include <format>
#include <iostream>

import Posix;

JackHandler::JackHandler(const std::filesystem::path & eventDev,
                         const std::filesystem::path & sndDev) :
    InputEvent(eventDev, File::Access::READ, EventType, EventCode),
    _hdaDev(sndDev)
{
}

void JackHandler::onEvent(const int value)
{
#ifdef __VERBOSE_LOG
    std::cout << std::format("{}: onEvent({})", ProgramName, value)
              << std::endl;
#endif
    value ? onJackInserted() : onJackRemoved();
}

void JackHandler::onInvalidEvent(const uint16_t type, const uint16_t code,
                                 const int value)
{
#ifdef __VERBOSE_LOG
    std::cout << std::format("{}: onInvalidEvent({}, {}, {})", ProgramName,
                             type, code, value)
              << std::endl;
#endif
}

/**
 * Verbs that should be send to the driver are listed in the Arch Linux
 * wiki: https://wiki.archlinux.org/title/Huawei_MateBook_14s#Headphones
 */

void JackHandler::onJackInserted()
{
    // Move the output to the headphones DAC
    _hdaDev.writeVerb(0x16, 0x701, 0x0);

    // Disable the speaker
    _hdaDev.writeVerb(0x17, 0x70C, 0x0);

    // Pin the output mode
    _hdaDev.writeVerb(0x1, 0x717, 0x2);

    // Enable the pin
    _hdaDev.writeVerb(0x1, 0x716, 0x2);

    // Clear the pin value
    _hdaDev.writeVerb(0x1, 0x715, 0x0);
}

void JackHandler::onJackRemoved()
{
    // Move the output to the speaker DAC
    _hdaDev.writeVerb(0x16, 0x701, 0x1);

    // Enable the speaker
    _hdaDev.writeVerb(0x17, 0x70C, 0x2);

    // Disable the headphones
    _hdaDev.writeVerb(0x1, 0x715, 0x2);
}
