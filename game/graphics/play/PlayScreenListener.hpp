#pragma once


#include "../common/DialogListener.hpp"
#include "../common/ButtonListener.hpp"

namespace awd::game {

    class PlayScreenListener :
            public DialogListener,
            public ButtonListener {
    private:
        //////////////////////////////////////////////////////////////////////////////////////////////
        //   Обработчики событий
        //////////////////////////////////////////////////////////////////////////////////////////////

        static void resumeClicked(Drawable* playDrawable);

        static void quitClicked(Drawable* playDrawable);

    public:
        void dialogOpened(Drawable* parentScreen, id_type dialogId) override;

        void dialogClosed(Drawable* parentScreen, id_type dialogId) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId) override;
    };

}

