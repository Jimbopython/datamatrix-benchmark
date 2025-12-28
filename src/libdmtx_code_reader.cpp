#include <sfdm/libdmtx_code_reader.hpp>
#include <dmtx.h>

namespace {
    class DecodeGuard {
    public:
        explicit DecodeGuard(const cv::Mat &image) : m_image(dmtxImageCreate(
                                                                 image.data, image.cols, image.rows, DmtxPack8bppK),
                                                             [](DmtxImage *dmtxImage) {
                                                                 dmtxImageDestroy(&dmtxImage);
                                                             }), m_decoder(dmtxDecodeCreate(m_image.get(), 1),
                                                                           [](DmtxDecode *decoder) {
                                                                               dmtxDecodeDestroy(&decoder);
                                                                           }) {
            if (!m_image) {
                throw std::runtime_error("Could not create image!");
            }

            if (!m_decoder) {
                throw std::runtime_error("Could not create decoder!");
            }
        }

        std::shared_ptr<DmtxDecode> getDecoder() {
            return m_decoder;
        }

    private:
        std::shared_ptr<DmtxImage> m_image;
        std::shared_ptr<DmtxDecode> m_decoder;
    };

    template<typename T, auto Callable>
    class GenericGuard {
    public:
        explicit GenericGuard(T *guardable) : m_guardable(guardable,
                                                          [](T *dmtxGuardable) {
                                                              Callable(&dmtxGuardable);
                                                          }) {
        }

        std::shared_ptr<T> getGuardable() {
            return m_guardable;
        }

    private:
        std::shared_ptr<T> m_guardable;
    };
}

namespace sfdm {
    std::vector<DecodeResult> LibdmtxCodeReader::decode(const cv::Mat &image) const {
        DecodeGuard decodeGuard(image);

        std::vector<DecodeResult> results;

        for (int i = 0; i < 255; ++i) {
            DmtxTime timeout = dmtxTimeNow();
            timeout = dmtxTimeAdd(timeout, m_timeoutMSec);
            const auto region = dmtxRegionFindNext(decodeGuard.getDecoder().get(), m_timeoutMSec ? &timeout : nullptr);
            if (!region) {
                break;
            }
            GenericGuard<DmtxRegion, dmtxRegionDestroy> regionGuard(region);

            DmtxMessage *message =
                    dmtxDecodeMatrixRegion(decodeGuard.getDecoder().get(), region, DmtxTrue);
            if (!message) {
                break;
            }
            GenericGuard<DmtxMessage, dmtxMessageDestroy> messageGuard(message);

            results.emplace_back(DecodeResult{reinterpret_cast<const char *>(message->output)});
        }

        return results;
    }

    void LibdmtxCodeReader::setTimeout(uint32_t msec) {
        m_timeoutMSec = msec;
    }

    bool LibdmtxCodeReader::isTimeoutSupported() {
        return true;
    }
}
