#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class WaterBackground : public Drawable {
    private:
        std::vector<std::shared_ptr<sf::RectangleShape>> flickers;
        std::vector<std::shared_ptr<sf::CircleShape>> bubbles;

        void createFlickers();
        void createBubbles();

        void updateFlickers();
        void updateBubbles();

    public:
        WaterBackground(int id,
                        float renderScale,
                        const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;
    };

}

