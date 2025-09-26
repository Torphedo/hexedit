#include "Base.h"
#include <string>

namespace Base {
    std::string toHex(int value, u32 lenAfterPad) {
        char buf[64] = {};
        snprintf(buf, sizeof(buf) - 1, "%0*x", lenAfterPad, value);
        return buf;
    }

    char toAscii(u8 byte) {
        return (byte > 32 && byte < 127) ? byte : '.';
    }

    u8 toByte(char *hex) {
        return strtoul(hex, nullptr, 16);
    }
}
