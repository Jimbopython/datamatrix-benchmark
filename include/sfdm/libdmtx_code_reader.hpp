#pragma once
#include <memory>
#include <sfdm/icode_reader.hpp>
#include <sfdm/result_stream.hpp>
#include <vector>

extern "C" {
struct DmtxRegion_struct;
struct DmtxDecode_struct;
struct DmtxMessage_struct;
}

namespace sfdm {
    /*!
     * Code Reader using libdmtx in the backend.
     * Best for accuracy, but can be slow, when timeout is not set or too high
     */
    class LibdmtxCodeReader : public ICodeReader {
    public:
        /*!
         * Decode damatrix codes in the provided image.
         * This is a blocking call until the decoding of all damatrix codes in the image are finished.
         * It is recommended to set the number of damatrix codes that can be detected, because then this function will
         * return faster. How fast the function "gives up" searching for codes in the image, can be tuned with the
         * setTimeout function.
         * @param image image used for damatrix code detection and decoding
         * @return Decoded results that were found in the image
         */
        [[nodiscard]] std::vector<DecodeResult> decode(const ImageView &image) const override;
        [[nodiscard]] std::vector<DecodeResult> decode(const ImageView &image,
                                                       std::function<void(DecodeResult)> callback) const override;

        [[nodiscard]] ResultStream<DecodeResult> decodeStream(const ImageView &image) const;

        /*!
         * This is a timeout that will be reset after each detection of one code in an image.
         * This timeout is for detection only, not decoding.
         * Note: 0 is until really nothing can be found
         * @param msec
         */
        void setTimeout(uint32_t msec) override;
        [[nodiscard]] uint32_t getTimeout() const override;
        bool isTimeoutSupported() override;

        void setMaximumNumberOfCodesToDetect(size_t count) override;
        [[nodiscard]] size_t getMaximumNumberOfCodesToDetect() const override;

        bool isDecodeWithCallbackSupported() override;

    protected:
        /*!
         * Libdmtx detection needs immediate decoding to be verified. The decoder is shared between detection and
         * decoding. When decoded successfully, the detection uses this info and skips nearby possible detections.
         * @param image
         * @return
         */
        [[nodiscard]] std::vector<DetectionResult> detect(const ImageView &image) const override;
        [[nodiscard]] ResultStream<DetectionResult> detectStream(const ImageView &image) const;

    private:
        enum class StopCause {
            ScanNotFound,
            ScanSuccess,
            ScanTimeLimit,
            ScanIterLimit,
        };

        [[nodiscard]] std::pair<std::shared_ptr<DmtxRegion_struct>, StopCause>
        detectNext(const std::shared_ptr<DmtxDecode_struct> &decoder) const;

        [[nodiscard]] std::shared_ptr<DmtxMessage_struct>
        decode(const std::shared_ptr<DmtxDecode_struct> &decoder,
               const std::shared_ptr<DmtxRegion_struct> &region) const;
        uint32_t m_timeoutMSec{200};
        size_t m_maximumNumberOfCodesToDetect{255};
    };
} // namespace sfdm
