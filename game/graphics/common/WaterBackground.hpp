#pragma once


#include "Drawable.hpp"
#include "RectWrapper.hpp"

namespace awd::game {

    class WaterBackground : public Drawable {
    private:
        std::vector<std::shared_ptr<RectWrapper    >> flickers;
        std::vector<std::shared_ptr<sf::CircleShape>> bubbles;

        void createFlickers();
        void createBubbles();

        void updateFlickers();
        void updateBubbles();

    public:
        explicit WaterBackground(id_type id);

        void update() override;
        void draw() override;
    };

}

