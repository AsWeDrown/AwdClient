#pragma once


#include <string>
#include "Drawable.hpp"

namespace awd::game {

    class Button : public Drawable {
    protected:
        bool enabled = true;

        virtual void onLeftClick();

    public:
        void update() override;
        void draw() override;
        void keyPressed(const sf::Event::KeyEvent &event) override;
        void mousePressed(const sf::Event::MouseButtonEvent &event) override;

        bool isEnabled() const;
        void setEnabled(bool nowEnabled);
    };

}
