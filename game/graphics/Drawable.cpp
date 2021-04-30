#include "Drawable.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PROTECTED
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Drawable::updateChildren() {
        for (const auto& child : children)
            child->update();
    }

    void Drawable::drawChildren() {
        for (const auto& child : children)
            child->draw();
    }

    bool Drawable::isMouseOver() const {
        return isMouseOver(sf::Mouse::getPosition(*window));
    }

    bool Drawable::isMouseOver(const sf::Vector2i& mousePos) const {
        return isMouseOver(mousePos.x, mousePos.y);
    }

    bool Drawable::isMouseOver(unsigned int mouseX, unsigned int mouseY) const {
        return mouseX >= x && mouseX <= x + width
            && mouseY >= y && mouseY <= y + height;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    unsigned int Drawable::getX() const {
        return x;
    }

    unsigned int Drawable::getY() const {
        return y;
    }

    unsigned int Drawable::getWidth() const {
        return width;
    }

    unsigned int Drawable::getHeight() const {
        return height;
    }

    void Drawable::keyPressed(const sf::Event::KeyEvent& event) {
        for (const auto& child : children)
            child->keyPressed(event);
    }

    void Drawable::mousePressed(const sf::Event::MouseButtonEvent& event) {
        for (const auto& child : children)
            child->mousePressed(event);
    }

    /**
     * Какой бы не была реализация update какого-то компонента Drawable,
     * ОБЯЗАТЕЛЬНО нужно вызывать родительский метод Drawable::update(),
     * чтобы система дочерних компонентов Drawable работала корректно.
     */
    void Drawable::update() {
        updateChildren();
    }

    /**
     * Какой бы не была реализация update какого-то компонента Drawable,
     * ОБЯЗАТЕЛЬНО нужно вызывать родительский метод Drawable::render(),
     * чтобы система дочерних компонентов Drawable работала корректно.
     */
    void Drawable::draw() {
        drawChildren();
    }

}
