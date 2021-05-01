#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class DialogListener {
    public:
        virtual void dialogOpened(Drawable* mainMenuScreen, id_type dialogId) = 0;
        virtual void dialogClosed(Drawable* mainMenuScreen, id_type dialogId) = 0;
    };

}

