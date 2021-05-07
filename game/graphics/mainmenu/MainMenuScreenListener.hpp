#pragma once


#include "../common/DialogListener.hpp"
#include "../common/ButtonListener.hpp"
#include "../common/TextFieldListener.hpp"

namespace awd::game {

    class MainMenuScreenListener :
            public DialogListener,
            public ButtonListener,
            public TextFieldListener {
    private:
        std::wstring enteredPlayerName,
                     enteredLobbyId;

        //////////////////////////////////////////////////////////////////////////////////////////////
        //   Утилити-методы
        //////////////////////////////////////////////////////////////////////////////////////////////

        static void requestPlayerNameInput(Drawable* mainMenuScreen);
        static void requestLobbyIdInput(Drawable* mainMenuScreen);

        //////////////////////////////////////////////////////////////////////////////////////////////
        //   Обработчики событий
        //////////////////////////////////////////////////////////////////////////////////////////////

        static void quitGameClicked(Drawable* mainMenuScreen);

        static void createLobbyClicked(Drawable* mainMenuScreen);
        static void createLobbyNextClicked(Drawable* dialog);
        static void createLobbyBackClicked(Drawable* dialog);

        static void joinLobbyClicked(Drawable* mainMenuScreen);
        static void joinLobbyNextClicked(Drawable* dialog);
        static void joinLobbyBackClicked(Drawable* dialog);

        static void errorOkClicked(Drawable* dialog);

    public:
        void dialogOpened(Drawable* parentScreen, id_type dialogId) override;

        void dialogClosed(Drawable* parentScreen, id_type dialogId) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId) override;

        void contentsChanged(Drawable* textFieldParent, id_type textFieldId,
                             const std::wstring& newContents) override;

        std::wstring getEnteredPlayerName() const;

        std::wstring getEnteredLobbyId() const;
    };

}

