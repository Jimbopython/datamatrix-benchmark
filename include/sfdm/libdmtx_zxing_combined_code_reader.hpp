#pragma once
#include <atomic>
#include <sfdm/icode_reader.hpp>
#include <sfdm/libdmtx_code_reader.hpp>
#include <sfdm/zxing_code_reader.hpp>
#include <vector>

namespace sfdm {
    class LibdmtxZXingCombinedCodeReader : public ICodeReader {
    public:
        [[nodiscard]] std::vector<DecodeResult> decode(const ImageView &image) const override;
        [[nodiscard]] std::vector<DecodeResult> decode(const ImageView &image,
                                                       std::function<void(DecodeResult)> callback) const override;

        void setTimeout(uint32_t msec) override;
        [[nodiscard]] uint32_t getTimeout() const override;
        bool isTimeoutSupported() override;

        void setMaximumNumberOfCodesToDetect(size_t count) override;
        [[nodiscard]] size_t getMaximumNumberOfCodesToDetect() const override;

        bool isDecodeWithCallbackSupported() override;

        void setDoubleCheckZXing(bool value);
        [[nodiscard]] bool getDoubleCheckZXing() const;

    private:
        LibdmtxCodeReader m_libdmtxCodeReader;
        ZXingCodeReader m_zxingCodeReader;
        std::atomic<bool> m_doubleCheckZXing{true};
    };
} // namespace sfdm
