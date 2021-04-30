#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    class Drawable {
    private:
        std::vector<std::shared_ptr<Drawable>> children;

        bool isChildIdLocallyUnique(int childId) const;

    protected:
        int id;
        float renderScale;
        unsigned int x, y, width, height;
        std::shared_ptr<sf::RenderWindow> window = nullptr;
        Drawable* parent = nullptr; // FIXME - насколько безопасно тут использовать обычный указатель?
        bool visible = true;

        void addChild(const std::shared_ptr<Drawable>& child);
        void removeChild(int childId);
        void updateChildren();
        void drawChildren();
        bool isMouseOver() const;
        bool isMouseOver(const sf::Vector2i& mousePos) const;
        bool isMouseOver(unsigned int mouseX, unsigned int mouseY) const;

    public:
        int getId() const;
        unsigned int getX() const;
        unsigned int getY() const;
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        bool isVisible() const;
        void setVisible(bool visibility);
        Drawable* getParent() const;
        std::shared_ptr<Drawable> getChildById(int childId) const;

        virtual void keyPressed(const sf::Event::KeyEvent& event);
        virtual void mousePressed(const sf::Event::MouseButtonEvent& event);
        virtual void update();
        virtual void draw();
    };

}
