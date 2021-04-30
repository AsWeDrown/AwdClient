#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    class Drawable {
    protected:
        float renderScale;
        unsigned int x, y, width, height;
        std::shared_ptr<sf::RenderWindow> window = nullptr;
        std::vector<std::shared_ptr<Drawable>> children;

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

        virtual void keyPressed(const sf::Event::KeyEvent& event);
        virtual void mousePressed(const sf::Event::MouseButtonEvent& event);
        virtual void update();
        virtual void draw();
    };

}
