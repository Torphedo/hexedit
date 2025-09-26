#pragma once
#include <string>

#include <common/int.h>

namespace Base {
    std::string toHex(int value, u32 lenAfterPad = 2);
    char toAscii(u8 byte);
    u8 toByte(char *hex);
}
