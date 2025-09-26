#pragma once

#include <string>
#include <map>

enum class SaveOption {
    SAME_FILE, NEW_FILE
};

struct Buffer {
    size_t fileSize = 0;
    std::string filePath;
    char* buffer = nullptr;
    bool modified = false;
    std::map<int, int> og;

    void load(const char* filePath) noexcept;
    void save(SaveOption option) noexcept;
    
    char at(std::size_t pos) const noexcept;
    char at(std::size_t pos, bool &modified) const noexcept;
    void set(std::size_t pos, char newByte) noexcept;
    bool isModified() const noexcept {
        return modified;
    }
    void revert() noexcept;
    void undo(std::size_t pos) noexcept;
    int ogVal(std::size_t pos) noexcept;

    ~Buffer();
};
