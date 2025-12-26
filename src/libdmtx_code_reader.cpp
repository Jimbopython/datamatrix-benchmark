#include <sfdm/libdmtx_code_reader.hpp>
#include <dmtx.h>

namespace sfdm {
    std::vector<DecodeResult> LibdmtxCodeReader::decode(const cv::Mat &image) {
        DmtxImage *dmtxImage = dmtxImageCreate(
            image.data, image.cols, image.rows, DmtxPack8bppK);
        if (!dmtxImage) return {};

        DmtxDecode *decoder = dmtxDecodeCreate(dmtxImage, 1);
        if (!decoder) {
            dmtxImageDestroy(&dmtxImage);
            return {};
        }

        std::vector<DecodeResult> results;

        for (int i = 0; i < 255; ++i) {
            DmtxRegion *region = dmtxRegionFindNext(decoder, nullptr);
            if (!region) {
                break;
            }

            DmtxMessage *message =
                    dmtxDecodeMatrixRegion(decoder, region, DmtxTrue);

            dmtxRegionDestroy(&region);
            if (!message) {
                break;
            }
            results.emplace_back(DecodeResult{reinterpret_cast<const char *>(message->output)});
            dmtxMessageDestroy(&message);
        }
        dmtxDecodeDestroy(&decoder);
        dmtxImageDestroy(&dmtxImage);

        return results;
    }
}
