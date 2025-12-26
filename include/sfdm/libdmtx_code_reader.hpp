#pragma once
#include <sfdm/icode_reader.hpp>

namespace sfdm {
    class LibdmtxCodeReader : public ICodeReader {
    public:
        std::vector<DecodeResult> decode(const cv::Mat &image) override;
    };
}
