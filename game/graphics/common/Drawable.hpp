#pragma once


#include <memory>
#include <SFML/Graphics.hpp>
#include <mutex>
#include "Identifiers.hpp"
#include "../../FatalErrors.hpp"

namespace awd::game {

    class Drawable {
    private:
        static std::recursive_mutex mutex;
        static std::vector<id_type> registeredIds;

        std::vector<std::shared_ptr<Drawable>> children, queuedChildren;

        void registerChild(const std::shared_ptr<Drawable>& child);

        static bool isIdUnique(id_type id);

    protected:
        // Иинициализируются в базовом конструкторе:
        id_type id;
        float renderScale;
        std::shared_ptr<sf::RenderWindow> window = nullptr;

        // Должны инициализироваться в дочерних конструкторах:
        unsigned int x, y, width, height;
        bool visible = true;
        bool enabled = true;

        // Не инициализируются (устанавливаются "потом"):
        Drawable* parent = nullptr;

        /**
         * Этот метод используется для регистрации дочерних компонентов вне цикла,
         * обновления т.е. исключительно при создании этого объекта Drawable. Он
         * регистрирует указанного потомка мгновенно. Соответственно, onRegister()
         * этого потомка будет вызван тоже мгновенно, без задержек.
         *
         * @see #enqueueChild
         */
        void addChild(const std::shared_ptr<Drawable>& child);

        /**
         * Этот метод используется для регистрации дочерних компонентов во время цикла
         * обновления или во время обработки событий, таких как mousePressed. Вместо
         * мгновенной регистрации указанного потомка, он добавляет его в очередь.
         * Объекты, помещённые в очередь, будут зарегистрированы и удалены из очереди
         * при следующем обновлении этого объекта Drawable (update). Тогда же (т.е. с
         * небольшой задержкой) будет вызван и метод onRegister() этого потомка.
         *
         * @see #queueChild
         */
        void enqueueChild(const std::shared_ptr<Drawable>& child);

        /**
         * Вызывается при регистрации этого объекта в списке потомков (children). Может
         * служить для проведения "финальных штрихов" в создании объекта. Более подробный
         * порядок вызова onRegister() можно найти в описании методов:
         *
         * @see #addChild (мгновенная регистрация)
         * @see #enqueueChild (отложенная регистрация)
         *
         * Для объектов Drawable самого высокого уровня (т.е. таких, которые сами не являются
         * потомками никаких других Drawable), этот метод никогда не будет вызван.
         */
        virtual void onRegister();

        void removeChild(id_type childId);

        void updateChildren();

        void drawChildren();

        bool isMouseOver() const;

        bool isMouseOver(const sf::Vector2i& mousePos) const;

        bool isMouseOver(unsigned int mouseX, unsigned int mouseY) const;

    public:
        Drawable(id_type id,
                 float renderScale,
                 const std::shared_ptr<sf::RenderWindow>& window);

        ~Drawable();

        id_type getId() const;
        float getRenderScale() const;
        std::shared_ptr<sf::RenderWindow> getWindow() const;
        unsigned int getX() const;
        unsigned int getY() const;
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        bool isVisible() const;
        void setVisible(bool visibility);
        bool isEnabled() const;
        void setEnabled(bool nowEnabled);
        Drawable* getParent() const;
        std::shared_ptr<Drawable> getChildById(id_type childId) const;
        std::shared_ptr<Drawable> getChildByIdRecursively(id_type childId) const;

        virtual void keyPressed(const sf::Event::KeyEvent& event);
        virtual void mousePressed(const sf::Event::MouseButtonEvent& event);
        virtual void update();
        virtual void draw();
    };

}
