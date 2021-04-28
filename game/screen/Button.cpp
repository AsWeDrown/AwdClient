#include "Button.hpp"

namespace awd::game {

    std::string Button::getText() const {
        return text;
    }

    void Button::setText(const std::string& newText) {
        this->text = newText;
    }

    bool Button::isEnabled() const {
        return enabled;
    }

    void Button::setEnabled(bool nowEnabled) {
        this->enabled = nowEnabled;
    }

}
