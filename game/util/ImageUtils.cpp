#include <cstdio>
#include "ImageUtils.hpp"


namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Pixels::Pixels(uint32_t bmpWidth, uint32_t bmpHeight,
                   const std::shared_ptr<unsigned char[]>& bmpData) {
        this->bmpWidth = bmpWidth;
        this->bmpHeight = bmpHeight;
        this->bmpData = bmpData;
    }

    int Pixels::rgbAt(uint32_t pixelX, uint32_t pixelY) const {
        if (pixelX >= bmpWidth)
            throw std::invalid_argument("pixelX out of bounds");

        if (pixelY >= bmpHeight)
            throw std::invalid_argument("pixelY out of bounds");

        uint32_t flippedPixelY = bmpHeight - pixelY - 1; // т.к. BMP хранит изображения перевёрнутыми по вертикали
        uint32_t pixelOffset   = 3 * (flippedPixelY * bmpWidth + pixelX);

        int r = bmpData[pixelOffset    ];
        int g = bmpData[pixelOffset + 1];
        int b = bmpData[pixelOffset + 2];

        int rgb =              r;
            rgb = (rgb << 8) + g;
            rgb = (rgb << 8) + b;

        return rgb;
    }

    std::shared_ptr<Pixels> ImageUtils::readBitmapImage(const char* bmpFilePath) {
        ///////////////////////////////////////////////////////////////////////////////////////////////
        //
        // https://stackoverflow.com/a/9296467/7837105
        //
        //   1. Система должна быть little-endian.
        //   2. Изображение должно использовать 24 бита для кодирования цветов.
        //   3. Ширина изображения должна быть кратна четырём (4).
        //
        ///////////////////////////////////////////////////////////////////////////////////////////////
        FILE* f = fopen(bmpFilePath, "rb");
        unsigned char info[54];
        fread(info, sizeof(unsigned char), 54, f);

        int width  = *(int*) &info[18];
        int height = *(int*) &info[22];
        int   size = 3 * width * height;

        auto data = std::shared_ptr<unsigned char[]>(new unsigned char[size]);
        fread(data.get(), sizeof(unsigned char), size, f);
        fclose(f);

        for (int i = 0; i < size; i += 3)
            std::swap(data[i], data[i + 2]);

        return std::make_shared<Pixels>(width, height, data);
    }

}
