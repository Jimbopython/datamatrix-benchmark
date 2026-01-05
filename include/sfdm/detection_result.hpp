#pragma once
#include <memory>
#include <sfdm/code_position.hpp>
#include <vector>

namespace sfdm {
    class DetectionResult {
    public:
        struct DetectionResultImpl {
            virtual ~DetectionResultImpl() = default;
        };
        DetectionResult() = default;
        ~DetectionResult();
        DetectionResult(const DetectionResult &other) = default;
        DetectionResult &operator=(const DetectionResult &other) = default;
        DetectionResult(DetectionResult &&) noexcept;
        DetectionResult &operator=(DetectionResult &&) noexcept;

        [[nodiscard]] CodePosition getPosition() const;
        [[nodiscard]] std::shared_ptr<DetectionResultImpl> getImpl() const;

    private:
        std::shared_ptr<DetectionResultImpl> m_impl;
        CodePosition m_codePosition;

        explicit DetectionResult(std::shared_ptr<DetectionResultImpl> impl, const CodePosition &codePosition);

        friend class ZXingCodeReader;
        friend class LibdmtxCodeReader;
        friend class LibdmtxZXingCombinedCodeReader;
    };

} // namespace sfdm
