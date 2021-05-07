#include <iostream>
#include "Drawable.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::recursive_mutex Drawable::mutex;
    std::vector<id_type> Drawable::registeredIds;

    void Drawable::registerChild(const std::shared_ptr<Drawable>& child) {
        children.push_back(child);
        child->onRegister();
    }

    bool Drawable::isIdUnique(id_type id) {
        return std::all_of(registeredIds.cbegin(), registeredIds.cend(),
                           [id](id_type otherId) { return otherId != id; });
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PROTECTED
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Drawable::addChild(const std::shared_ptr<Drawable>& child) {
        std::unique_lock<std::recursive_mutex> lock(mutex);
        child->parent = this;
        registerChild(child); // мгновенная регистрация
    }

    void Drawable::enqueueChild(const std::shared_ptr<Drawable>& child) {
        std::unique_lock<std::recursive_mutex> lock(mutex);
        child->parent = this;
        queuedChildren.push_back(child); // отложенная регистрация
    }

    void Drawable::onRegister() {}

    void Drawable::removeChild(id_type childId) {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        children.erase(std::remove_if(
                children.begin(), children.end(),
                [childId](const auto& child) {
                    return child->id == childId;
                }), children.end()
        );
    }

    void Drawable::updateChildren() {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (const auto& child : children)
            child->update();
    }

    void Drawable::drawChildren() {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (const auto& child : children)
            if (child->visible)
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
        std::unique_lock<std::recursive_mutex> lock(mutex);

        if (!isIdUnique(id)) {
            std::wcerr << L"ID not unique: " << id << std::endl;
            throw std::invalid_argument(DRAWABLE_ID_NOT_UNIQUE);
        }

        // Инициализируем самые базовые (основные, обязательные) поля.
        this->id = id;
        this->renderScale = renderScale;
        this->window = window;

        // Занимаем этот ID.
        registeredIds.push_back(id);
    }

    Drawable::~Drawable() {
        std::unique_lock<std::recursive_mutex> lock(mutex);

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
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (const auto& child : children)
            if (child->id == childId)
                return child;

        return nullptr;
    }

    std::shared_ptr<Drawable> Drawable::getChildByIdRecursively(id_type childId) const {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (const auto& child : children) {
            if (child->id == childId)
                return child;

            for (const auto& childOfChild : child->children) {
                if (childOfChild->id == childId)
                    return child;

                if (auto found = childOfChild->getChildByIdRecursively(childId))
                    return found;
            }
        }

        return nullptr;
    }

    void Drawable::keyPressed(const sf::Event::KeyEvent& event) {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (const auto& child : children)
            if (child->enabled)
                child->keyPressed(event);
    }

    void Drawable::mousePressed(const sf::Event::MouseButtonEvent& event) {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        for (const auto& child : children)
            if (child->enabled)
                child->mousePressed(event);
    }

    /**
     * Какой бы не была реализация update какого-то компонента Drawable,
     * ОБЯЗАТЕЛЬНО нужно вызывать родительский метод Drawable::update(),
     * чтобы система дочерних компонентов Drawable работала корректно.
     */
    void Drawable::update() {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        // Регистрируем все дочерние компоненты, созданные во время события,
        // произошедшего между этим и предыдущим тиком. Подробное объяснение
        // необходимости этого см. над объявлением метода #enqueueChild (.hpp).
        if (!queuedChildren.empty()) {
            for (const auto& queuedChild : queuedChildren)
                registerChild(queuedChild);

            queuedChildren.clear();
        }

        updateChildren();
    }

    /**
     * Какой бы не была реализация update какого-то компонента Drawable,
     * ОБЯЗАТЕЛЬНО нужно вызывать родительский метод Drawable::render(),
     * чтобы система дочерних компонентов Drawable работала корректно.
     */
    void Drawable::draw() {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        if (visible)
            drawChildren();
    }

}
