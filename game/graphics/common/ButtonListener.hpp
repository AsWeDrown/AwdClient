#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class ButtonListener {
    public:
        virtual void buttonClicked(Drawable* buttonParent, id_type buttonId) = 0;
    };

}

