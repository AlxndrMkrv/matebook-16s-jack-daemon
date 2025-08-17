#include "Globals.hxx"
#include "JackHandler.hxx"
#include "OutPipeEnd.hxx"
#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

import Posix;
import Alsa;

namespace fs = std::filesystem;

extern fs::path find_device_node(const std::string_view devname_base,
                                 const std::string_view prop_filename,
                                 const std::string_view property,
                                 const std::string_view dev_directory,
                                 const std::string_view sys_directory);

// Global pipes to transmit signal emittion from handler to poll() function
matebook::posix::Pipe sig_in_pipe{matebook::posix::Pipe::Type::WRITE};
OutPipeEnd sig_out_pipe;

void handle_signal(int signo)
{
    const std::string msg = std::format(
        "Signal {} received", matebook::posix::signal::SigName(signo));
    sig_in_pipe.write(msg);
}

void setup_event_poll()
{
    using namespace matebook::posix;
    using namespace matebook::alsa;
    // Block all signals
    signal::BlockAll();

    // Connect signal handler pipe ends
    Pipe::Connect(sig_in_pipe, sig_out_pipe);

    // Find path to the /dev/input/eventX file associated with Headphone events
    // of the sof-hda-dsp driver
    const fs::path event_filename = find_device_node(
        "event", "device/name", EventSource, "/dev/input", "/sys/class/input");

    // Find path to the /dev/snd/hwCxDy file associated with SN6140 SoC
    const fs::path snd_filename = find_device_node(
        "hw", "chip_name", SoC, "/dev/snd", "/sys/class/sound");

    // Create the jack event object that will handle jack insertions/removings
    JackHandler jh{event_filename, snd_filename};

    // Create the Poll object that will wait until either the event on
    // /dev/input/eventX happen or POSIX signal will come
    // Poll<2> poll{{event_filename, snd_filename}, &sig_out_pipe};
    Poll<2> poll{std::initializer_list<Pollable *>{&jh, &sig_out_pipe}};

    // Assign signals to the handling function
    signal::Assign(signal::Type::INT, handle_signal);
    signal::Assign(signal::Type::TERM, handle_signal);

    // Get the current jack state from Alsa mixer
    // Suppose that the daemon is started at startup and jack switch is inverted
    IsJackConnected() ? jh.onJackInserted() : jh.onJackRemoved();

#ifdef __VERBOSE_LOG
    std::cout << std::format("{}: jack is {}", ProgramName,
                             IsJackConnected() ? "connected" : "disconnected")
              << std::endl;
#endif

    // Run the event polling loop
    poll.runUntilTimeout();

    // This message shouldn't appear
    std::cerr << std::format("{} event loop stopped unexpectedly", ProgramName)
              << std::endl;
}

int main()
{
    try {
        setup_event_poll();
    } catch (const std::runtime_error & e) {
        std::cerr << std::format("{} stopped by exception: {}", ProgramName,
                                 e.what())
                  << std::endl;
        return 1;
    }
}
