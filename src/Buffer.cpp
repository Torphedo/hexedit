#include "Buffer.h"

#include <fstream>
#include <string>
#include <map>

namespace Buffer {
    char *buffer;
    std::string filePath;
    size_t fileSize;
    bool modified = false;
    std::map<int, int> og;

    void load(std::string file) {
        filePath = file;
        std::ifstream ifs(file, std::ios::in | std::ios::binary);

        ifs.seekg(0, std::ios::end);
        fileSize = ifs.tellg();
        buffer = new char[fileSize];

        ifs.seekg(0, std::ios::beg);
        ifs.read(buffer, fileSize);
        ifs.close();
    }

    void save(SaveOption option) {
        std::string outFilePath;
        if (option == SaveOption::SAME_FILE) {
            outFilePath = filePath;
        } else {
            outFilePath = "mod_" + filePath + "";
        }

        std::ofstream ofs(outFilePath, std::ios::out | std::ios::binary);
        ofs.write(buffer, fileSize);
        ofs.close();
        modified = false;
    }

    size_t size() { return fileSize; }

    char at(size_t pos) {
        if (pos > fileSize)
            throw std::range_error("Buffer::at()");
        else
            return buffer[pos];
    }

    char at(size_t pos, bool &modified) {
        modified = og.find(pos) != og.end();
        return at(pos);
    }

    void set(size_t pos, char newByte) {
        if (pos > fileSize)
            throw std::range_error("Buffer::set()");

        if (buffer[pos] == newByte)
            return;

        auto iter = og.find(pos);
        if (iter == og.end())             // first change
            og[pos] = buffer[pos];
        else if (iter->second == newByte) // changing back to og
            og.erase(pos);
        else {}                           // changed, but changing more

        buffer[pos] = newByte;
        modified = !og.empty();
    }

    void revert() {
        for (auto iter : og)
            buffer[iter.first] = iter.second;

        og.clear();
        modified = false;
    }

    void undo(std::size_t pos) {
        if (og.find(pos) == og.end())
            return;
            
        buffer[pos] = og.find(pos)->second;
        og.erase(pos);
        modified = !og.empty();
    }

    bool isModified() { return modified; }
    int ogVal(size_t pos) { return og[pos]; }

    void finish() {
        delete[] buffer;
        buffer = nullptr;
    }
}
