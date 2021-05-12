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

        std::vector<std::shared_ptr<Drawable>> children,
                                               childrenAddQueue;

        std::vector<id_type> childrenRemoveQueue;

        void registerChild(const std::shared_ptr<Drawable>& child);

        static void takeId(id_type idToTake);

        static void freeId(id_type idToFree);

        static bool isIdUnique(id_type id);

    protected:
        // Иинициализируются в базовом конструкторе:
        id_type id;
        float renderScale;
        std::shared_ptr<sf::RenderWindow> window = nullptr;

        // Инициализируются в дочерних конструкторах:
        float x, y, width, height;

        // Инициализируются "оптимальными" значениями по-умолчанию
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
         * @see #enqueueAddChild (отложенная регистрация)
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
         * ПРИМЕЧАНИЕ: объектам Drawable, созданным через enqueueAdd, стоит создавать
         *             свои дочерние компоненты через всё тот же enqueueAdd.
         *
         * @see #queueChild (мгновенная регистрация)
         */
        void enqueueAddChild(const std::shared_ptr<Drawable>& child);

        /**
         * Вызывается при регистрации этого объекта в списке потомков (children). Может
         * служить для проведения "финальных штрихов" в создании объекта. Более подробный
         * порядок вызова onRegister() можно найти в описании методов:
         *
         * @see #addChild (мгновенная регистрация)
         * @see #enqueueAddChild (отложенная регистрация)
         *
         * Для объектов Drawable самого высокого уровня (т.е. таких, которые сами не являются
         * потомками никаких других Drawable), этот метод никогда не будет вызван.
         */
        virtual void onRegister();

        /**
         * Мгновенно удаляет потомка с указанным ID. Нельзя использовать
         * внутри цикла обновления и цикла обработки событий.
         *
         * Система аналогична #addChild и #enqueueAddChild.
         *
         * @see #enqueueRemoveChild (отложенное удаление)
         */
        void removeChild(id_type childId);

        /**
         * Отложенно удаляет потомка с указанным ID. Можно использовать
         * внутри цикла обновления и цикла обработки событий.
         *
         * Система аналогична #addChild и #enqueueAddChild.
         *
         * @see #removeChild (мгновенное удаление)
         */
        void enqueueRemoveChild(id_type childId);

        void updateChildren();

        void drawChildren();

        bool isMouseOver() const;

        bool isMouseOver(const sf::Vector2i& mousePos) const;

        bool isMouseOver(float mouseX, float mouseY) const;

    public:
        explicit Drawable(id_type id);
        ~Drawable();

        id_type getId() const;
        float getRenderScale() const;
        std::shared_ptr<sf::RenderWindow> getWindow() const;
        float getX() const;
        float getY() const;
        float getWidth() const;
        float getHeight() const;
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
