#pragma once


#include <string>
#include <stdexcept>

namespace awd::game {

    class CrashReporter {
    public:
        static void makeReport(const std::string& message                          ) noexcept;
        static void makeReport(const std::string& message, const std::exception& ex) noexcept;
    };

}
