#include <iostream>
#include "Drawable.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::recursive_mutex Drawable::mutex;
    std::vector<id_type> Drawable::registeredIds;

    void Drawable::registerChild(const std::shared_ptr<Drawable>& child) {
        takeId(child->id); // занимаем этот ID (чтобы не появилось других объектов с таким же номером)
        children.push_back(child);
        child->onRegister();
    }

    void Drawable::takeId(id_type idToTake) {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        if (isIdUnique(idToTake))
            registeredIds.push_back(idToTake);
        else {
            std::wcerr << L"Drawable ID not unique: " << idToTake << std::endl;
            throw std::invalid_argument(DRAWABLE_ID_NOT_UNIQUE);
        }
    }

    void Drawable::freeId(id_type idToFree) {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        registeredIds.erase(std::remove_if(
                registeredIds.begin(), registeredIds.end(),
                [idToFree](id_type otherId) {
                    return otherId == idToFree;
                }), registeredIds.end()
        );
    }

    bool Drawable::isIdUnique(id_type id) {
        std::unique_lock<std::recursive_mutex> lock(mutex);
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
        registerChild(child);
    }

    void Drawable::enqueueAddChild(const std::shared_ptr<Drawable>& child) {
        std::unique_lock<std::recursive_mutex> lock(mutex);
        child->parent = this;
        childrenAddQueue.push_back(child); // отложенная регистрация
    }

    void Drawable::onRegister() {}

    void Drawable::removeChild(id_type childId) {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        freeId(childId); // освобождаем ID мгновенно (вдруг деструктор вызовется с задержкой?)
        children.erase(std::remove_if(
                children.begin(), children.end(),
                [childId](const std::shared_ptr<Drawable>& child) {
                    return child->id == childId;
                }), children.end()
        );
    }

    void Drawable::enqueueRemoveChild(id_type childId) {
        std::unique_lock<std::recursive_mutex> lock(mutex);
        childrenRemoveQueue.push_back(childId); // отложенное удаление
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

    bool Drawable::isMouseOver(float mouseX, float mouseY) const {
        return mouseX >= x && mouseX <= x + width
            && mouseY >= y && mouseY <= y + height;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Drawable::Drawable(id_type id) { // NOLINT(cppcoreguidelines-pro-type-member-init)
        std::unique_lock<std::recursive_mutex> lock(mutex);

        // Инициализируем самые базовые (основные, обязательные) поля.
        this->id = id;
        this->renderScale = Game::instance().getRenderScale();
        this->window = Game::instance().getWindow();
    }

    Drawable::~Drawable() {
        freeId(id); // освбождаем этот ID, если это не было сделано ранее (дополнительная мера предосторожности)
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

    float Drawable::getX() const {
        return x;
    }

    float Drawable::getY() const {
        return y;
    }

    float Drawable::getWidth() const {
        return width;
    }

    float Drawable::getHeight() const {
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

        // Отложенное удаление потомков (см. подробное описание чуть ниже).
        // Удаление должно происходить ДО добавления, чтобы у родительских
        // объектов Drawable была возможность "заменять" одного потомка на
        // другого с таким же ID во время цикла обновления или событий.
        if (!childrenRemoveQueue.empty()) {
            for (const auto& queuedChild : childrenRemoveQueue)
                removeChild(queuedChild);

            childrenRemoveQueue.clear();
        }

        // Регистрируем все дочерние компоненты, созданные во время события,
        // произошедшего между этим и предыдущим тиком. Подробное объяснение
        // необходимости этого см. над объявлением метода #enqueueAddChild (.hpp).
        if (!childrenAddQueue.empty()) {
            for (const auto& queuedChild : childrenAddQueue)
                registerChild(queuedChild);

            childrenAddQueue.clear();
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
