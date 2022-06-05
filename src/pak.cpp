// Copyright (C) 2022 Zach Collins <zcollins4@proton.me>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <filesystem>
#include <fstream>
#include "pakfile.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [DIRECTORY PATHS...]" << std::endl;
        return 1;
    }

    for (auto i = 1; i < argc; i++) {
        std::string path = argv[i];
        std::cout << "Processing " << path << std::endl;

        // Make sure that it is a directory
        if (!std::filesystem::is_directory(std::filesystem::path(path))) {
            std::cerr << "Error: " << path << " is not a directory" << std::endl;
            return 1;
        }

        if (path.back() == '/' || path.back() == '\\')
            path.pop_back();

        try {
            PakFile pak(path + ".pak", PakFile::Create);

            // Loop through the files
            for (const auto &dir_entry : std::filesystem::recursive_directory_iterator(std::filesystem::path(path))) {
                if (!dir_entry.is_regular_file())
                    continue;

                // Read the data
                std::ifstream file(dir_entry.path().string(), std::ios::in | std::ios::binary | std::ios::ate);
                std::size_t size = file.tellg();
                auto data = std::make_unique<std::uint8_t[]>(size);

                file.seekg(0, std::ios::beg);
                file.read(reinterpret_cast<char*>(data.get()), size);
                file.close();

                // Write the data to the Pak
                auto name = dir_entry.path().string().substr(path.size()+1);
                pak.write_entry(name, data.get(), size);
            }
        }

        catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
