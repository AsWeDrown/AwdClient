#include <iostream>
#include "ConfigManager.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool ConfigManager::loadGameConfig() {
        try {
            game = GameConfig();
            game.load();

            return true;
        } catch (const std::runtime_error& ex) {
            std::wcerr << L"Failed to load config: game: " << ex.what() << std::endl;
            return false;
        }
    }

    bool ConfigManager::loadPhysicsConfig() {
        try {
            physics = PhysicsConfig();
            physics.load();

            return true;
        } catch (const std::runtime_error& ex) {
            std::wcerr << L"Failed to load config: physics: " << ex.what() << std::endl;
            return false;
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool ConfigManager::loadConfigs() {
        std::wcout << L"Configuring" << std::endl;

        return loadGameConfig   ()
            && loadPhysicsConfig();
    }

}
