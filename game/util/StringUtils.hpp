#pragma once


#include <string>
#include <vector>
#include "../graphics/common/ColorSet.hpp"

namespace awd::game {

    class StringUtils {
    public:
        static std::wstring encodeFormatting(std::wstring message);

        static std::wstring fixSpacing(std::wstring message);

        static std::wstring wrapByLineLength(std::wstring message,
                                             uint32_t maxLineLen, uint32_t maxLinesNum);
    };

}

