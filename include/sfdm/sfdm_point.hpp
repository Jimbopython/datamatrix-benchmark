#pragma once
#include <cstdint>
namespace sfdm {
    struct Point {
        uint32_t x{};
        uint32_t y{};
        Point(uint32_t x, uint32_t y) : x{x}, y{y} {}
        Point() = default;
        auto operator<=>(const Point &) const = default;
        Point(const Point &other) = default;
        Point(Point &&other) noexcept = default;
        Point &operator=(const Point &other) = default;
        Point &operator=(Point &&other) noexcept = default;
    };
} // namespace sfdm
