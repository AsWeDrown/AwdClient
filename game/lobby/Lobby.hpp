#pragma once


#include <cstdint>
#include <string>
#include <map>

namespace awd::game {

    struct Lobby {
        uint32_t     lobbyId;
        uint32_t     hostId;
        uint32_t     ownPlayerId;
        std::wstring ownPlayerName;
        uint32_t     ownCharacter;

        std::map<uint32_t, std::wstring> playerNames;
        std::map<uint32_t,     uint32_t> playerCharacters;

        /////////////////////////////////////////////////////////////////////////////////////////////////

        uint32_t ownEntityId;
    };

}

