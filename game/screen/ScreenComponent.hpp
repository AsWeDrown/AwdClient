#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    enum class ScreenComponentType {
        BACKGROUND,
        LABEL,
        BUTTON
    };

    class ScreenComponent {
    protected:
        ScreenComponentType type;
        std::shared_ptr<sf::RenderWindow> window = nullptr;

    public:
        ScreenComponent(ScreenComponentType type,
                        std::shared_ptr<sf::RenderWindow>& window);

        ScreenComponentType getType() const;
        virtual void render() = 0;
    };

}
