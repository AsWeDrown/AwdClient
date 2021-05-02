#pragma once


#include <string>
#include "Drawable.hpp"
#include "ButtonListener.hpp"

namespace awd::game {

    class Button : public Drawable {
    protected:
        std::shared_ptr<ButtonListener> listener = nullptr;

    public:
        Button(id_type id,
               float renderScale,
               const std::shared_ptr<sf::RenderWindow>& window,
               const std::shared_ptr<ButtonListener>& listener);

        void update() override;
        void draw() override;
        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
    };

}
