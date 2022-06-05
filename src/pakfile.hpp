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
