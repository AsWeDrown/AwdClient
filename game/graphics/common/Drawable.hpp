#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    class Drawable {
    private:
        bool parentPropagated = false;

        void propagateParent();
        void ensureParentPropagated();

    protected:
        float renderScale;
        unsigned int x, y, width, height;
        std::shared_ptr<sf::RenderWindow> window = nullptr;
        Drawable* parent = nullptr; // FIXME - насколько безопасно тут использовать обычный указатель?
        std::vector<std::shared_ptr<Drawable>> children;
        bool visible = true;

        void updateChildren();
        void drawChildren();
        bool isMouseOver() const;
        bool isMouseOver(const sf::Vector2i& mousePos) const;
        bool isMouseOver(unsigned int mouseX, unsigned int mouseY) const;

    public:
        unsigned int getX() const;
        unsigned int getY() const;
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        bool isVisible() const;
        void setVisible(bool visibility);
        Drawable* getParent() const;

        virtual void keyPressed(const sf::Event::KeyEvent& event);
        virtual void mousePressed(const sf::Event::MouseButtonEvent& event);
        virtual void update();
        virtual void draw();
    };

}
