#pragma once
#include <sfdm/sfdm_point.hpp>

namespace sfdm {
    struct CodePosition {
        Point bottomLeft{};
        Point topLeft{};
        Point topRight{};
        Point bottomRight{};
        CodePosition() = default;
        CodePosition(Point bottomLeft, Point topLeft, Point topRight, Point bottomRight) :
            bottomLeft{bottomLeft}, topLeft{topLeft}, topRight{topRight}, bottomRight{bottomRight} {}
        auto operator<=>(const CodePosition &) const = default;
        CodePosition(const CodePosition &other) = default;
        CodePosition(CodePosition &&other) noexcept = default;
        CodePosition &operator=(const CodePosition &other) = default;
        CodePosition &operator=(CodePosition &&other) noexcept = default;
    };
} // namespace sfdm
