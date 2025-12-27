#pragma once
#include <sfdm/icode_reader.hpp>

namespace sfdm {
    class LibdmtxCodeReader : public ICodeReader {
    public:
        [[nodiscard]] std::vector<DecodeResult> decode(const cv::Mat &image) const override;
    };
}
