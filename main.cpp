#include <catch2/catch_test_macros.hpp>
#include <dmtx.h>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <ZXing/ReadBarcode.h>

bool containsAll(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    return std::all_of(b.begin(), b.end(), [&](const auto& element) {
        return std::find(a.begin(), a.end(), element) != a.end();
    });
}

std::map<std::string, std::vector<std::string>> readDataMatrixFile(const std::string& filename)
{
    std::map<std::string, std::vector<std::string>> result;
    std::ifstream file(filename);

    if (!file.is_open())
        return result;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        const auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string values = line.substr(pos + 1);

        std::stringstream ss(values);
        std::string token;

        while (std::getline(ss, token, '|'))
        {
            if (!token.empty())
                result[key].emplace_back(token);
        }
    }

    return result;
}

void dmtx(const cv::Mat& cvImage, const std::vector<std::string>& data ) {
    // Create libdmtx image
    DmtxImage* image = dmtxImageCreate(
        cvImage.data,
        cvImage.cols,
        cvImage.rows,
        DmtxPack8bppK  // 8-bit grayscale
    );

    if (!image) {
        throw std::runtime_error("Failed to create image");
    }

    // Create decoder
    DmtxDecode* decoder = dmtxDecodeCreate(image, 1);
    if (!decoder) {
        dmtxImageDestroy(&image);
        throw std::runtime_error("Failed to create decoder");
    }

    std::vector<std::string> tmpData;

    for (int foundCodeCount = 0; foundCodeCount < data.size(); ++foundCodeCount) {
        CAPTURE(foundCodeCount, data.size());

        // Find the next Data Matrix symbol
        DmtxRegion* region = dmtxRegionFindNext(decoder, nullptr);
        REQUIRE(region != nullptr);
        // Decode the symbol
        DmtxMessage* message = dmtxDecodeMatrixRegion(
            decoder,
            region,
            DmtxUndefined
        );

        REQUIRE(message != nullptr);

        auto it = std::ranges::find(data, reinterpret_cast<const char*>(message->output));
        REQUIRE(it != data.end());
        tmpData.emplace_back(*it);

        dmtxMessageDestroy(&message);

        dmtxRegionDestroy(&region);
    }

    REQUIRE(containsAll(tmpData, data));

    // Cleanup
    dmtxDecodeDestroy(&decoder);
    dmtxImageDestroy(&image);
}

void zxing(const cv::Mat& cvImage, const std::vector<std::string>& data) {
    ZXing::ImageView source{cvImage.data, cvImage.cols, cvImage.rows, ZXing::ImageFormat::Lum};

    ZXing::ReaderOptions options;
    options.setBinarizer(ZXing::Binarizer::LocalAverage);
    options.setTryHarder(true);
    const auto results = ZXing::ReadBarcodes(source, options);

    std::vector<std::string> tmpData;
    int foundCount = 0;
    for (const auto& result : results) {
        ++foundCount;
        CAPTURE(foundCount, data.size());
        auto it = std::ranges::find(data, result.text());
        REQUIRE(it != data.end());
        tmpData.emplace_back(*it);
    }
    REQUIRE(tmpData.size() == data.size());
    REQUIRE(containsAll(tmpData, data));
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    const auto data = readDataMatrixFile("_deps/images-src/annotations.txt");

    for (const auto& entry : std::filesystem::directory_iterator("_deps/images-src")) {
        if (entry.is_regular_file() && entry.path().extension() == ".jpg") {
            cv::Mat image = cv::imread(entry.path().string(), cv::IMREAD_COLOR);

            // Convert to 8-bit grayscale
            cv::Mat gray;
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

            // Ensure it's 8-bit (usually already is)
            gray.convertTo(gray, CV_8U);

            const auto fileName = entry.path().stem().string();
            const auto it = data.find(fileName);
            if (it == data.end()) {
                continue;
            }
            SECTION("LibDMTX - " + fileName) {
                dmtx(gray, it->second);
            }
            SECTION("ZXing - " + fileName) {
                zxing(gray, it->second);
            }
        }
    }
}