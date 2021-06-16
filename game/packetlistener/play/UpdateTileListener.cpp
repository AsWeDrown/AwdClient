#include "UpdateTileListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void UpdateTileListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::UpdateTile>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY)
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen))
                playScreen->getWorld()->getTerrainControls()->
                        replaceTileAt(packet->x(), packet->y(), packet->new_tile_id());
    }

}
