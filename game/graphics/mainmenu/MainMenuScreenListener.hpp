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
        std::wstring enteredPlayerName;

        static void createLobbyClicked(Drawable* mainMenuScreen);
        static void createLobbyNextClicked(Drawable* dialog);
        static void createLobbyBackClicked(Drawable* dialog);

    public:
        void dialogOpened(Drawable* mainMenuScreen, id_type dialogId) override;

        void dialogClosed(Drawable* mainMenuScreen, id_type dialogId) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId) override;

        void contentsChanged(Drawable* textFieldParent, id_type textFieldId,
                             const std::wstring& newContents) override;

        std::wstring getEnteredPlayerName() const;
    };

}

