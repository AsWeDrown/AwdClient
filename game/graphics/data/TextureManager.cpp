#include <iostream>
#include "TextureManager.hpp"
#include "../../entity/Entities.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool TextureManager::loadCharAnim(uint32_t character, uint32_t animation) {
        auto texture = std::make_shared<sf::Texture>();
        std::string file = "assets/textures/characters/char"
                "_" + std::to_string(character) + "/anim_" + std::to_string(animation) + ".png";

        if (!texture->loadFromFile(file)) {
            std::wcerr << L"Failed to load character texture: " << character << L"_" << animation << std::endl;
            return false;
        }

        characters[character][animation] = texture;

        return true;
    }

    bool TextureManager::loadCharacters() {
        for (int i = 1; i <= 4; i++) { // 4 персонажа
            if (!loadCharAnim(i, Entities::EntityPlayer::ANIM_BASE_STILL_FRONT )) return false;
            if (!loadCharAnim(i, Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_0)) return false;
            if (!loadCharAnim(i, Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_1)) return false;
            if (!loadCharAnim(i, Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_2)) return false;
        }

        return true;
    }

    bool TextureManager::loadWorldTileMap() {
        worldTileMap = std::make_shared<sf::Texture>();
        return worldTileMap->loadFromFile("assets/textures/world/tilemap.png");
    }

    bool TextureManager::loadEndgameTextures() {
        endgameSuccess = std::make_shared<sf::Texture>();
        endgameFailureSomeoneDied = std::make_shared<sf::Texture>();
        endgameServerError = std::make_shared<sf::Texture>();

        return endgameSuccess->           loadFromFile("assets/textures/endgame/success.png"             )
            && endgameFailureSomeoneDied->loadFromFile("assets/textures/endgame/failure_someone_died.png")
            && endgameServerError->       loadFromFile("assets/textures/endgame/server_error.png"        )
            ;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool TextureManager::loadTextures() {
        std::wcout << L"Loading textures" << std::endl;
        std::wcout << L"Maximum texture size allowed by current graphics card: "
                   << sf::Texture::getMaximumSize() << std::endl;

        return loadCharacters     ()
            && loadWorldTileMap   ()
            && loadEndgameTextures();
    }

}
