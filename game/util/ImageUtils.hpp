#pragma once


#include <memory>

namespace awd::game {

    class Pixels {
    public:
        uint32_t bmpWidth, bmpHeight;
        std::shared_ptr<unsigned char[]> bmpData;

        Pixels(uint32_t bmpWidth, uint32_t bmpHeight,
               const std::shared_ptr<unsigned char[]>& bmpData);

        int rgbAt(uint32_t pixelX, uint32_t pixelY) const;
    };

    class ImageUtils {
    public:
        static std::shared_ptr<Pixels> readBitmapImage(const char* bmpFilePath);
    };

}

