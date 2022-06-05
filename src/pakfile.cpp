#include "pakfile.hpp"
#include "common.hpp"
#include <algorithm>

Pak::Pak(const std::string &path, Mode mode) : mode_(mode) {
    if (mode == Mode::Open)
        open(path);
    else
        create(path);
}

Pak::~Pak() {
    // Only for saving
    if (mode_ != Mode::Create)
        return;

    // Create the header
    Header header;
    header.id[0] = 'P'; header.id[1] = 'A'; header.id[2] = 'C'; header.id[3] = 'K';
    header.offset = Common::little32(file.tellp());
    header.size   = Common::little32(entries.size() * sizeof(Entry));

    // Save the entries
    for (auto i = 0; i < entries.size(); i++) {
        Entry entry = entries[i];

        // Endian swap the entry
        entry.offset = Common::little32(entry.offset);
        entry.size   = Common::little32(entry.size);

        file.write(reinterpret_cast<char*>(&entry), sizeof(Entry));
    }

    // Now save the header
    file.seekg(0, std::ios::beg);
    file.write(reinterpret_cast<char*>(&header), sizeof(Header));
    file.close();
}

std::size_t Pak::num_entries() const {
    return entries.size();
}

std::string Pak::entry_name(std::size_t index) const {
    if (index >= entries.size())
        return "";

    return std::string(entries[index].name);
}

std::size_t Pak::entry_size(std::size_t index) const {
    if (index >= entries.size())
        return 0;

    return entries[index].size;
}

bool Pak::valid() {
    if (mode_ != Mode::Open)
        return false;

    file.seekg(0, std::ios::end);
    std::size_t size = file.tellg();

    for (const auto &entry : entries) {
        // Make sure that the entry is contained inside
        if (entry.offset + entry.size > size)
            return false;

        // Make sure that the name is null terminated
        for (int i = 55; i >= 0; i--) {
            if (entry.name[i])
                return false;
            else
                break;
        }
    }

    return true;
}

std::unique_ptr<std::uint8_t[]> Pak::read_entry(std::size_t index) {
    if (mode_ != Mode::Open || index >= entries.size())
        return nullptr;

    auto data = std::make_unique<std::uint8_t[]>(entries[index].size);

    // Read the data
    file.seekg(entries[index].offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.get()), entries[index].size);

    return data;
}

bool Pak::write_entry(const std::string &name, const void *data, std::size_t size) {
    // Including null-terminator
    if (mode_ != Mode::Create || name.size() >= sizeof(Entry::name))
        return false;

    // Create the entry
    Entry entry;
    entry.offset = file.tellp();
    entry.size   = size;

    // Copy the name
    std::fill_n(entry.name, sizeof(entry.name), 0x00);
    std::copy_n(&name[0], name.size(), entry.name);
    entries.push_back(entry);

    // Write the data
    file.write(reinterpret_cast<const char*>(data), size);

    return true;
}

void Pak::open(const std::string &path) {
    // Read the header
    file = std::fstream(path, std::ios::in | std::ios::binary);
    if (!file.good())
        throw std::runtime_error("Unable to open file " + path);

    Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(Header));

    // Make sure that the file is a PAK
    if (std::string(header.id, 4) != "PACK")
        throw std::runtime_error("File " + path + " is not a PAK");

    header.size   = Common::little32(header.size);
    header.offset = Common::little32(header.offset);

    file.seekg(header.offset);

    entries.resize(header.size / sizeof(Entry));

    // Read the entries
    for (auto &entry : entries) {
        file.read(reinterpret_cast<char*>(&entry), sizeof(Entry));

        entry.offset = Common::little32(entry.offset);
        entry.size   = Common::little32(entry.size);
    }
}

void Pak::create(const std::string &path) {
    // Create the file
    file = std::fstream(path, std::ios::out | std::ios::binary);
    if (!file.good())
        throw std::runtime_error("Unable to create file " + path);

    // Skip the space where the header goes (We'll fill it in later)
    file.seekg(sizeof(Header), std::ios::beg);
}
