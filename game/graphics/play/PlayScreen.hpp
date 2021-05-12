#pragma once


#include "../common/Screen.hpp"
#include "PlayScreenListener.hpp"

namespace awd::game {

    class PlayScreen : public Screen {
    private:
        std::shared_ptr<PlayScreenListener> listener = std::make_shared<PlayScreenListener>();

    public:
        PlayScreen();

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        std::shared_ptr<PlayScreenListener> getListener() const;
    };

}

