#pragma once


#include "../common/DialogListener.hpp"
#include "../common/ButtonListener.hpp"

namespace awd::game {

    class LobbyScreenListener :
            public DialogListener,
            public ButtonListener {
    public:
        void dialogOpened(Drawable* parentScreen, id_type dialogId) override;

        void dialogClosed(Drawable* parentScreen, id_type dialogId) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId) override;
    };

}

