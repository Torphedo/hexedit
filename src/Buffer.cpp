#include "Buffer.h"

#include <fstream>
#include <string>
#include <map>
#include <common/file.h>

void Buffer::load(const char* file) noexcept {
    filePath = file;
    fileSize = file_size(file);
    buffer = (char*)file_load(file);
}

void Buffer::save(SaveOption option) noexcept {
    std::string outFilePath;
    switch (option) {
    case SaveOption::SAME_FILE:
        outFilePath = filePath;
        break;
    case SaveOption::NEW_FILE:
        outFilePath = "mod_" + filePath + "";
        break;
    }

    FILE* f = fopen(outFilePath.c_str(), "wb");
    if (f) {
        fwrite(buffer, fileSize, 1, f);
        fclose(f);
    }
    modified = false;
}

char Buffer::at(size_t pos) const noexcept {
    if (pos > fileSize) {
        throw std::range_error("Buffer::at()");
    } else {
        return buffer[pos];
    }
}

char Buffer::at(size_t pos, bool &modified) const noexcept {
    modified = og.find(pos) != og.end();
    return at(pos);
}

void Buffer::set(size_t pos, char newByte) noexcept {
    if (pos > fileSize) {
        throw std::range_error("Buffer::set()");
    }

    if (buffer[pos] == newByte) {
        return;
    }

    auto iter = og.find(pos);
    if (iter == og.end()) {           // first change
        og[pos] = buffer[pos];
    }
    else if (iter->second == newByte) { // changing back to og
        og.erase(pos);
    }

    buffer[pos] = newByte;
    modified = !og.empty();
}

void Buffer::revert() noexcept {
    for (auto iter : og) {
        buffer[iter.first] = iter.second;
    }

    og.clear();
    modified = false;
}

void Buffer::undo(std::size_t pos) noexcept {
    if (og.find(pos) == og.end()) {
        return;
    }

    buffer[pos] = og.find(pos)->second;
    og.erase(pos);
    modified = !og.empty();
}

int Buffer::ogVal(size_t pos) noexcept {
    return og[pos];
}

Buffer::~Buffer() {
    free(buffer);
}
