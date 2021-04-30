#include "Button.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PROTECTED
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Button::onLeftClick() {}

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
            onLeftClick();
    }

    bool Button::isEnabled() const {
        return enabled;
    }

    void Button::setEnabled(bool nowEnabled) {
        this->enabled = nowEnabled;
    }

}
