#pragma once


#include "../common/Button.hpp"
#include "../common/RichText.hpp"

namespace awd::game {

    class PlayerCard : public Button {
    private:
        uint32_t fontSize;
        bool highlighted = false;

        uint32_t displayedCharacter = 9999; // 9999 --> не рисуем ничего вообще; нужно исп. updateDisplayedCharacter
        std::wstring displayedText;

        std::unique_ptr<sf::RectangleShape> cardRect        = nullptr;

        std::unique_ptr<sf::Sprite>         characterSprite = nullptr;
        std::unique_ptr<sf::RectangleShape> characterPanel  = nullptr;
        std::unique_ptr<sfe::RichText>      characterText   = nullptr;

        std::unique_ptr<sf::Text>           placeholderText = nullptr;

    public:
        PlayerCard(id_type id,
                   float x, float y,
                   float width, float height,
                   const std::shared_ptr<ButtonListener>& listener,
                   uint32_t fontSize);

        void update() override;
        void draw() override;

        void updateHighlighted(bool isHighlighted);
        void updateDisplayedCharacter(uint32_t character);
        void updateDisplayedText(const std::wstring& text);
    };

}

