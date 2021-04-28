#pragma once


#include <string>
#include "ScreenComponent.hpp"

namespace awd::game {

    class Button : public ScreenComponent {
    protected:
        std::string text;
        bool enabled;

    public:
        void render() override = 0;

        virtual void mouseEntered() = 0;
        virtual void mouseExited() = 0;
        virtual void buttonClicked() = 0;

        std::string getText() const;
        void setText(const std::string& newText);
        bool isEnabled() const;
        void setEnabled(bool nowEnabled);
    };

}
