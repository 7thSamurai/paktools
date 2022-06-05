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

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>

class PakFile
{
public:
    enum Mode {
        Open,
        Create
    };

    PakFile(const std::string &path, Mode mode);
    ~PakFile();

    std::size_t num_entries() const;
    std::string entry_name(std::size_t index) const;
    std::size_t entry_size(std::size_t index) const;

    bool valid();

    std::unique_ptr<std::uint8_t[]> read_entry(std::size_t index);
    bool write_entry(const std::string &name, const void *data, std::size_t size);

private:
    struct Header {
        char id[4];
        std::uint32_t offset;
        std::uint32_t size;
    };

    struct Entry {
        char name[56];
        std::uint32_t offset;
        std::uint32_t size;
    };

    void open(const std::string &path);
    void create(const std::string &path);

    Mode mode_;

    std::vector<Entry> entries;
    std::fstream file;
};
