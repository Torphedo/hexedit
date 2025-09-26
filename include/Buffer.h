#pragma once

#include <string>

enum class SaveOption {
    SAME_FILE, NEW_FILE
};

namespace Buffer {
    void load(std::string filePath);
    void save(SaveOption option);
    
    std::size_t size();
    char at(std::size_t pos);
    char at(std::size_t pos, bool &modified);
    void set(std::size_t pos, char newByte);
    bool isModified();
    void revert();
    void undo(std::size_t pos);
    int ogVal(std::size_t pos);

    void finish();
}
