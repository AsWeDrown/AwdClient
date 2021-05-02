#include <iostream>
#include "Button.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    
    Button::Button(id_type id,
                   float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window,
                   const std::shared_ptr<ButtonListener>& listener)
                   : Drawable(id, renderScale, window) {
        this->listener = listener;
    }

    void Button::update() {
        Drawable::update();
    }

    void Button::draw() {
        Drawable::draw();
    }

    void Button::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);
    }

    void Button::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);

        if (event.button == sf::Mouse::Button::Left && isMouseOver(event.x, event.y))
            listener->buttonClicked(parent, id);
    }

}
