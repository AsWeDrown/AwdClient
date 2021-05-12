#pragma once


#include "../common/Screen.hpp"
#include "PlayScreenListener.hpp"
#include "../../world/World.hpp"

namespace awd::game {

    class PlayScreen : public Screen {
    private:
        std::shared_ptr<PlayScreenListener> listener = std::make_shared<PlayScreenListener>();
        std::shared_ptr<World> world = nullptr;

    public:
        PlayScreen();

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        std::shared_ptr<PlayScreenListener> getListener() const;
        std::shared_ptr<World> getWorld() const;
    };

}

