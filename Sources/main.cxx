#include "EventDev.hxx"
#include "Globals.hxx"
#include "Poll.hxx"
#include "Terminate.hxx"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

fs::path find_event_dev(const std::string_view driver_name)
{
    const fs::directory_entry ev_dir{fs::path(DevInputDir)};
    if (!ev_dir.exists() || !ev_dir.is_directory()) {
        terminate(std::format("\"{}\" directory not found", DevInputDir));
    }

    const fs::directory_entry sys_dir{fs::path(SysInputDir)};
    if (!sys_dir.exists() || !sys_dir.is_directory()) {
        terminate(std::format("\"{}\" directory not found", SysInputDir));
    }

    for (const auto & entry : fs::directory_iterator(ev_dir)) {
        if (!entry.is_character_file())
            continue;

        const fs::path & filename = entry.path().filename();
        if (filename.string().rfind("event", 0) != 0)
            continue;

        const fs::path device_name =
            sys_dir / filename / fs::path("device/name");
        if (!fs::exists(device_name) || !fs::is_regular_file(device_name))
            continue;

        std::ifstream ifs{device_name.string()};
        std::string driver;
        std::getline(ifs, driver);
        ifs.close();

        if (driver.rfind(driver_name, 0) != 0)
            continue;

        return entry.path();
    }

    return fs::path();
}

int main()
{
    const fs::path dev_filename = find_event_dev(DeviceName);

    if (dev_filename.empty())
        terminate(std::format(
            "could not find the input device associated with \"{}\" driver",
            DeviceName));

    EventDeviceDescriptor fd{dev_filename};

    if (!fd.ready())
        terminate(std::format("failed to open {}: {}", dev_filename.string(),
                              fd.error()));

    Poll poll{fd, 500};

    while (!StopRequested) {

        switch (poll.run()) {
        case Poll::Result::TIMEOUT:
            break;
        }
    }
}
