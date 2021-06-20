#include <iostream>
#include "Button.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    
    Button::Button(id_type id,
                   const std::shared_ptr<ButtonListener>& listener)
                   : Drawable(id) {
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
            click();
    }

    void Button::click() {
        Game::instance().getSoundSystem()->playSound(Sound::BUTTON_CLICK);
        listener->buttonClicked(parent, id);
    }

}
