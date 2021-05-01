#include <iostream>
#include "Drawable.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::vector<id_type> Drawable::registeredIds;

    bool Drawable::isIdUnique(id_type id) {
        return std::all_of(registeredIds.cbegin(), registeredIds.cend(),
                           [id](id_type otherId) { return otherId != id; });
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

    Drawable::Drawable(id_type id, // NOLINT(cppcoreguidelines-pro-type-member-init)
                       float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window) {
        if (!isIdUnique(id)) {
            std::cerr << "ID not unique: " << id << std::endl;
            throw std::invalid_argument("ID not unique: " + std::to_string(id));
        }

        // Инициализируем самые базовые (основные, обязательные) поля.
        this->id = id;
        this->renderScale = renderScale;
        this->window = window;

        // Занимаем этот ID.
        registeredIds.push_back(id);
    }

    Drawable::~Drawable() {
        // Освобождаем этот ID.
        registeredIds.erase(std::remove_if(
                registeredIds.begin(), registeredIds.end(),
                [this](id_type otherId) {
                    return otherId == this->id;
                }), registeredIds.end()
        );
    }

    id_type Drawable::getId() const {
        return id;
    }

    float Drawable::getRenderScale() const {
        return renderScale;
    }

    std::shared_ptr<sf::RenderWindow> Drawable::getWindow() const {
        return window;
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

    bool Drawable::isEnabled() const {
        return enabled;
    }

    void Drawable::setEnabled(bool nowEnabled) {
        this->enabled = nowEnabled;
    }

    Drawable* Drawable::getParent() const {
        return parent;
    }

    std::shared_ptr<Drawable> Drawable::getChildById(id_type childId) const {
        for (const auto& child : children)
            if (child->id == childId)
                return child;

        return nullptr;
    }

    void Drawable::addChild(const std::shared_ptr<Drawable>& child) {
        child->parent = this;
        children.push_back(child);
    }

    void Drawable::removeChild(id_type childId) {
        children.erase(std::remove_if(
                children.begin(), children.end(),
                [childId](const auto& child) {
                    return child->id == childId;
                }), children.end()
        );
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
