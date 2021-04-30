#include "Drawable.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Drawable::propagateParent() {
        for (const auto& child : children)
            child->parent = this;
    }

    void Drawable::ensureParentPropagated() {
        if (!parentPropagated) {
            parentPropagated = true;
            propagateParent();
        }
    }

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

    bool Drawable::isVisible() const {
        return visible;
    }

    void Drawable::setVisible(bool visibility) {
        this->visible = visibility;
    }

    Drawable* Drawable::getParent() const {
        return parent;
    }

    void Drawable::keyPressed(const sf::Event::KeyEvent& event) {
        ensureParentPropagated();

        for (const auto& child : children)
            child->keyPressed(event);
    }

    void Drawable::mousePressed(const sf::Event::MouseButtonEvent& event) {
        ensureParentPropagated();

        for (const auto& child : children)
            child->mousePressed(event);
    }

    /**
     * Какой бы не была реализация update какого-то компонента Drawable,
     * ОБЯЗАТЕЛЬНО нужно вызывать родительский метод Drawable::update(),
     * чтобы система дочерних компонентов Drawable работала корректно.
     */
    void Drawable::update() {
        ensureParentPropagated();
        updateChildren();
    }

    /**
     * Какой бы не была реализация update какого-то компонента Drawable,
     * ОБЯЗАТЕЛЬНО нужно вызывать родительский метод Drawable::render(),
     * чтобы система дочерних компонентов Drawable работала корректно.
     */
    void Drawable::draw() {
        ensureParentPropagated();
        drawChildren();
    }

}
