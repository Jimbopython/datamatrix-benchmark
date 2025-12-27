#pragma once
#include <sfdm/icode_reader.hpp>

namespace sfdm {
    class LibdmtxFastCodeReader : public ICodeReader {
    public:
        [[nodiscard]] std::vector<DecodeResult> decode(const cv::Mat &image) const override;
    };
}
