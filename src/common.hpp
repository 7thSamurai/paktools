#pragma once

#include <endian.h>

namespace Common {

std::uint32_t little32(std::uint32_t n) {
#if BYTE_ORDER == BIG_ENDIAN
    return (n & 0xFF000000) >> 24 |
           (n & 0x00FF0000) >> 8  |
           (n & 0x0000FF00) << 8  |
           (n & 0x000000FF) << 24;
#else
    return n;
#endif
}

};
