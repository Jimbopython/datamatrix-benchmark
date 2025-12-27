find_package(Catch2 REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(test main.cpp test_utils.hpp test_utils.cpp)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain opencv::opencv sfdm)

include(FetchContent)
FetchContent_Declare(
        images
        URL https://mdpi-res.com/d_attachment/jimaging/jimaging-07-00163/article_deploy/jimaging-07-00163-s001.zip?version=1630062264
)

FetchContent_MakeAvailable(images)
