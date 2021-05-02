#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class DialogListener {
    public:
        virtual void dialogOpened(Drawable* dialogParent, id_type dialogId) = 0;
        virtual void dialogClosed(Drawable* dialogParent, id_type dialogId) = 0;
    };

}

