#pragma once


#include "../common/Screen.hpp"

namespace awd::game {

    class EndgameScreen : public Screen {
    private:
        std::unique_ptr<sf::Sprite> sprite;

    public:
        explicit EndgameScreen(uint32_t gameEndStatus);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void update() override;
        void draw() override;
    };

}

