#include <thread>
#include <fstream>
#include "game/Game.hpp"
#include "game/util/CrashReporter.hpp"

int main() {
    try {
        awd::game::Game::instance().bootstrap();
    } catch (const std::exception& ex) {
        std::wcerr << L"Fatal error: " << ex.what() << std::endl;
        std::wcerr << L"Exiting in 5 seconds..." << std::endl;
        awd::game::CrashReporter::makeReport("Unhandled exception in main()", ex);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::wcerr << L"Bye! :(" << std::endl;
    }
}