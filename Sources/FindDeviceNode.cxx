#include <filesystem>
#include <format>
#include <fstream>

namespace fs = std::filesystem;

std::filesystem::path find_device_node(const std::string_view devname_base,
                                       const std::string_view prop_filename,
                                       const std::string_view property,
                                       const std::string_view dev_directory,
                                       const std::string_view sys_directory)
{
    const fs::directory_entry dev_dir{fs::path(dev_directory)};
    if (!dev_dir.exists() || !dev_dir.is_directory())
        throw std::runtime_error(
            std::format("Failed to find a device node associated with \"{}\": "
                        "\"{}\" directory not found",
                        property, dev_directory));

    const fs::directory_entry sys_dir{fs::path(sys_directory)};
    if (!sys_dir.exists() || !sys_dir.is_directory())
        throw std::runtime_error(
            std::format("Failed to find a device node associated with \"{}\": "
                        "\"{}\" directory not found",
                        property, sys_directory));

    for (const auto & entry : fs::directory_iterator(dev_dir)) {
        if (!entry.is_character_file())
            continue;

        const fs::path & filename = entry.path().filename();
        if (filename.string().rfind(devname_base, 0) != 0)
            continue;

        const fs::path device_name =
            sys_dir / filename / fs::path(prop_filename);
        if (!fs::exists(device_name) || !fs::is_regular_file(device_name))
            continue;

        std::ifstream ifs{device_name.string()};
        std::string driver;
        std::getline(ifs, driver);
        ifs.close();

        if (driver.rfind(property, 0) != 0)
            continue;

        return entry.path();
    }

    throw std::runtime_error(
        std::format("Failed to find a device node in {} associated with \"{}\"",
                    dev_directory, property));
}
