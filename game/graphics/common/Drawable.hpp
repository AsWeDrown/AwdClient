#pragma once


#include <memory>
#include <SFML/Graphics.hpp>
#include "Identifiers.hpp"

namespace awd::game {

    class Drawable {
    private:
        static std::vector<id_type> registeredIds;
        std::vector<std::shared_ptr<Drawable>> children;

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
        Drawable* parent = nullptr; // FIXME - насколько безопасно тут использовать обычный указатель?

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
        void addChild(const std::shared_ptr<Drawable>& child);
        void removeChild(id_type childId);

        virtual void keyPressed(const sf::Event::KeyEvent& event);
        virtual void mousePressed(const sf::Event::MouseButtonEvent& event);
        virtual void update();
        virtual void draw();
    };

}
