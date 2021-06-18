#define SEP_LINE "==========================================================================================\n"
#define SPACE_NL " \n"


#include <iostream>
#include <fstream>
#include "CrashReporter.hpp"
#include "TimeUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void CrashReporter::makeReport(const std::string& message) noexcept {
        makeReport(message, std::runtime_error("< no exception >"));
    }

    void CrashReporter::makeReport(const std::string& message, const std::exception& ex) noexcept {
        std::string text;

        text += SPACE_NL;
        text += SEP_LINE;
        text += "  As We Drown [Client] - Crash Report\n";
        text += SEP_LINE;
        text += SPACE_NL;
        text += "  As We Drown game client terminated unexpectedly. Please report this\n";
        text += "  problem to developers. Make sure to provide this file in your report.\n";
        text += SPACE_NL;
        text += SEP_LINE;
        text += SPACE_NL;
        text += "  Current time millis: " + std::to_string(TimeUtils::currentTimeMillis()) + "\n";
        text += "  Message: " + message + "\n";
        text += "  Exception: " + std::string(ex.what()) + "\n";
        text += SPACE_NL;
        text += SEP_LINE;
        text += SPACE_NL;

        std::wcerr << std::wstring(text.begin(), text.end());

        try {
            std::ofstream file("AsWeDrown-Client-Crash-Report.txt");
            file << text;
        } catch (const std::exception& ex) {
            std::wcerr << L"Failed to save crash report in file: " << ex.what() << std::endl;
        }
    }

}

