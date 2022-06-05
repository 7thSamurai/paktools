// Copyright (C) 2022 Zach Collins <zcollins4@proton.me>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
