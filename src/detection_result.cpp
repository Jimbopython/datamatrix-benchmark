#include <sfdm/detection_result.hpp>

namespace sfdm {
    DetectionResult::DetectionResult(std::shared_ptr<DetectionResultImpl> impl, const CodePosition &codePosition) :
        m_impl(std::move(impl)), m_codePosition(codePosition) {}

    DetectionResult::DetectionResult(DetectionResult &&) noexcept = default;
    DetectionResult &DetectionResult::operator=(DetectionResult &&) noexcept = default;
    CodePosition DetectionResult::getPosition() const { return m_codePosition; }
    std::shared_ptr<DetectionResult::DetectionResultImpl> DetectionResult::getImpl() const { return m_impl; }
    DetectionResult::~DetectionResult() = default;
} // namespace sfdm
