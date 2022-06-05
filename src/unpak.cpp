#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
#include "pakfile.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [PAK PATHS...]" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        std::string path = argv[i];
        std::cout << "Extracting " << path << "..." << std::endl;

        try {
            Pak pak(path, Pak::Open);
            std::set<std::string> dirs;

            // Create the base directory
            auto base_path = std::filesystem::path(path).stem().string() + "/";
            std::filesystem::create_directory(std::filesystem::path(base_path));

            // Find all the directories
            for (auto j = 0; j < pak.num_entries(); j++) {
                auto name = pak.entry_name(j);
                auto pos  = name.find_last_of('/');

                // If there are no directories in this path
                if (pos == std::string::npos)
                    continue;

                dirs.insert(name.substr(0, pos + 1));
            }

            // Create the nessary directories
            for (const auto &dir : dirs)
                std::filesystem::create_directories(std::filesystem::path(base_path + dir));

            // Extract the entries
            for (auto j = 0; j < pak.num_entries(); j++) {
                auto name = pak.entry_name(j);
                auto size = pak.entry_size(j);
                auto data = pak.read_entry(j);

                // Write the entry
                std::ofstream file(base_path + name, std::ios::out | std::ios::binary);
                if (!file.good()) {
                    std::cerr << "Error creating file " << name << std::endl;
                    return 1;
                }

                file.write(reinterpret_cast<char*>(data.get()), size);
                file.close();
            }
        }
        catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
