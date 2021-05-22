#pragma once


#include <memory>
#include <SFML/Graphics.hpp>
#include "../graphics/common/RichText.hpp"

namespace awd::game {

    class RenderUtils {
    private:
        static void applyFormatting(sfe::RichText& msg, wchar_t code);

    public:
        static void enrichText(sfe::RichText& msg, const std::wstring& message);

        static void verticalGradientSquare(const std::shared_ptr<sf::RenderWindow>& window,
                                           float startX, float startY, float endX, float endY,
                                           const sf::Color& topColor, const sf::Color& bottomColor);

        static void turnLeft(const std::shared_ptr<sf::Sprite>& sprite);

        static void turnRight(const std::shared_ptr<sf::Sprite>& sprite);

        static void turnTop(const std::shared_ptr<sf::Sprite>& sprite);

        static void turnBottom(const std::shared_ptr<sf::Sprite>& sprite);
    };

}
