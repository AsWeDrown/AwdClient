#include "Drawable.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    /**
     * @throws std::invalid_argument если в потомках этого объекта уже есть объект с id = childId.
     */
    bool Drawable::isChildIdLocallyUnique(int childId) const {
        return getChildById(childId) == nullptr;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PROTECTED
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Drawable::addChild(const std::shared_ptr<Drawable>& child) {
        if (isChildIdLocallyUnique(child->id)) {
            children.push_back(child);
            child->parent = this;
        }
    }

    void Drawable::removeChild(int childId) {
        children.erase(std::remove_if(
                children.begin(), children.end(),
                [childId](const auto& child) {
                    return child->id == childId;
                }), children.end()
        );
    }

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

    int Drawable::getId() const {
        return id;
    }

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

    std::shared_ptr<Drawable> Drawable::getChildById(int childId) const {
        for (const auto& child : children)
            if (child->id == childId)
                return child;

        return nullptr;
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
