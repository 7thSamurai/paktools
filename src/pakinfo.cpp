#include <iostream>
#include <iomanip>
#include "pakfile.hpp"

std::string convert_size(unsigned int bytes) {
    if (bytes >= 1024*1024)
        return std::to_string(bytes / (1024*1024)) + "MiB";
    else if (bytes >= 1024)
        return std::to_string(bytes / 1024) + "KiB";

    return std::to_string(bytes) + "B";
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [PAK PATHS...]" << std::endl;
        return 1;
    }

    for (auto i = 1; i < argc; i++) {
        std::string path = argv[i];

        try {
            Pak pak(path, Pak::Open);

            // Find the total size of all the files
            unsigned int total_size = 0;
            for (auto i = 0; i < pak.num_entries(); i++)
                total_size += pak.entry_size(i);

            // Find the maximum length of the entry names
            std::size_t max_len = 0;
            for (auto j = 0; j < pak.num_entries(); j++) {
                auto len = pak.entry_name(j).size() + convert_size(pak.entry_size(j)).size();

                if (len > max_len)
                    max_len = len;
            }

            std::cout << std::setfill('-') << std::setw(max_len+2) << "-" << std::endl;
            std::cout << std::setfill(' ') << "File: " << path << std::endl;
            std::cout << "Valid: " << (pak.valid() ? "Yes" : "No") << std::endl;
            std::cout << "Number of files: " << pak.num_entries() << std::endl;
            std::cout << "Total file size: " << convert_size(total_size) << "\n" << std::endl;

            // List the entries
            for (auto j = 0; j < pak.num_entries(); j++) {
                auto name = pak.entry_name(j);
                auto size = pak.entry_size(j);

                std::cout << name << "  ";
                std::cout << std::setw(max_len - name.size()) << convert_size(size) << "\n";
            }

            std::cout << std::setfill('-') << std::setw(max_len+3) << "-\n" << std::endl;
        }
        catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
