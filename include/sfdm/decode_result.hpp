#pragma once
#include <sfdm/code_position.hpp>
#include <string>
#include <utility>

namespace sfdm {
    struct DecodeResult {
        std::string text;
        CodePosition position{};
        DecodeResult() = default;
        DecodeResult(std::string text, const CodePosition &position) : text{std::move(text)}, position{position} {}
        auto operator<=>(const DecodeResult &) const = default;
        DecodeResult(const DecodeResult &other) = default;
        DecodeResult(DecodeResult &&other) noexcept = default;
        DecodeResult &operator=(const DecodeResult &other) = default;
        DecodeResult &operator=(DecodeResult &&other) noexcept = default;
    };
} // namespace sfdm
