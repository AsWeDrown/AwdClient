#include "UpdatedMembersListListener.hpp"
#include "../Game.hpp"

namespace awd::game {

    void UpdatedMembersListListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::UpdatedMembersList>(basePacket);
        auto lobby = Game::instance().getCurrentLobby();

        if (lobby != nullptr) {
            lobby->playerNames.clear();
            lobby->playerCharacters.clear();

            for (const auto& [playerId, playerName] : packet->new_all_names())
                lobby->playerNames[playerId] = std::wstring(
                        playerName.begin(), playerName.end());

            for (const auto& [playerName, playerCharacter] : packet->new_all_characters())
                lobby->playerCharacters[playerName] = playerCharacter;
        }
    }

}
