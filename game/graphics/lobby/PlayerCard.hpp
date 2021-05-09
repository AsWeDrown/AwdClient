#pragma once


#include "../common/Button.hpp"

namespace awd::game {

    class PlayerCard : public Button {
    private:
        uint32_t fontSize;

        uint32_t displayedCharacter = 0;

        std::unique_ptr<sf::Sprite> characterSprite = nullptr;

    public:
        PlayerCard(id_type id,
                   float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window,
                   float x, float y,
                   float width, float height,
                   const std::shared_ptr<ButtonListener>& listener,
                   uint32_t fontSize);

        void update() override;
        void draw() override;

        uint32_t getDisplayedCharacter() const;
        void setDisplayedCharacter(uint32_t character);
    };

}

