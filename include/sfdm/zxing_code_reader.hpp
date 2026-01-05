#pragma once
#include <memory>
#include <sfdm/icode_reader.hpp>

namespace sfdm {
    struct ZXingCodeReaderImpl;


    class ZXingCodeReader : public ICodeReader {
    public:
        ZXingCodeReader();

        ~ZXingCodeReader() override;

        [[nodiscard]] std::vector<DetectionResult> detect(const ImageView &image) const override;

        [[nodiscard]] std::vector<DecodeResult> decode(const ImageView &image) const override;
        [[nodiscard]] std::vector<DecodeResult> decode(const ImageView &image,
                                                       std::function<void(DecodeResult)> callback) const override;
        void setTimeout(uint32_t msec) override;
        [[nodiscard]] uint32_t getTimeout() const override;
        bool isTimeoutSupported() override;

        void setMaximumNumberOfCodesToDetect(size_t count) override;
        size_t getMaximumNumberOfCodesToDetect() const override;

        bool isDecodeWithCallbackSupported() override;

    private:
        std::unique_ptr<ZXingCodeReaderImpl> m_impl;
    };
} // namespace sfdm
