#pragma once


#include "../common/DialogListener.hpp"
#include "../common/ButtonListener.hpp"
#include "../common/NoPayload.hpp"

namespace awd::game {

    class MainMenuScreenListener :
            public DialogListener,
            public ButtonListener<std::wstring>,
            public ButtonListener<NoPayload> {
    private:
        static void createLobbyClicked(Drawable* mainMenuScreen);

        static void createLobbyNextClicked(Drawable* dialog,
                                           const std::wstring& playerName);

        static void createLobbyBackClicked(Drawable* dialog);

    public:
        void dialogOpened(Drawable* mainMenuScreen, id_type dialogId) override;

        void dialogClosed(Drawable* mainMenuScreen, id_type dialogId) override;

        void buttonClicked(Drawable* mainMenuScreen, id_type buttonId,
                           const std::wstring& payload) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId,
                           const NoPayload& payload) override;
    };

}

