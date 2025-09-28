#pragma once

#include <string>
#include <map>

#include <common/int.h>

enum class SaveOption {
    SAME_FILE, NEW_FILE
};

struct Buffer {
    size_t fileSize = 0;
    std::string filePath;
    u8* buffer = nullptr;
    bool modified = false;
    std::map<int, int> og;

    void load(const char* filePath) noexcept;
    void save(SaveOption option) noexcept;
    
    u8 at(std::size_t pos) const noexcept;
    u8 at(std::size_t pos, bool &modified) const noexcept;
    void set(std::size_t pos, u8 newByte) noexcept;
    void revert() noexcept;
    void undo(std::size_t pos) noexcept;
    int ogVal(std::size_t pos) noexcept;

    ~Buffer();
};
