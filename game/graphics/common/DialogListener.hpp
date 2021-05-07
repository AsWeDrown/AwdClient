#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class DialogListener {
    public:
        virtual void dialogOpened(Drawable* parentScreen, id_type dialogId) = 0;
        virtual void dialogClosed(Drawable* parentScreen, id_type dialogId) = 0;
    };

}

