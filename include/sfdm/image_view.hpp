#pragma once
#include <cstdint>

namespace sfdm {
    // currently only 8 bit Mono
    struct ImageView {
        size_t width;
        size_t height;
        uint8_t *data;
    };
} // namespace sfdm
