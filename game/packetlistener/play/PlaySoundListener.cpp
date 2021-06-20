#include "PlaySoundListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void PlaySoundListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::PlaySound>(packetData->getPacket());
        Game::instance().getSoundSystem()->playSound(game::Sound(
                packet->sound_id(), packet->source_x(), packet->source_y()));
    }

}
