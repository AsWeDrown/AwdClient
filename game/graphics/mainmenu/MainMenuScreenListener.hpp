#pragma once


#include <packets.pb.h>
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

        static void beginCreateLobby(Drawable* mainMenuScreen,
                                     const std::wstring& playerName);

        static void beginJoinLobby(Drawable* mainMenuScreen,
                                   uint32_t lobbyId,
                                   const std::wstring& playerName);

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
        static void finishCreateLobby(Drawable* mainMenuScreen,
                                      const std::shared_ptr<net::CreateLobbyResponse>& response);

        static void finishJoinLobby(Drawable* mainMenuScreen,
                                    const std::shared_ptr<net::JoinLobbyResponse>& response);

        void dialogOpened(Drawable* parentScreen, id_type dialogId) override;

        void dialogClosed(Drawable* parentScreen, id_type dialogId) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId) override;

        void contentsChanged(Drawable* textFieldParent, id_type textFieldId,
                             const std::wstring& newContents) override;

        std::wstring getEnteredPlayerName() const;

        std::wstring getEnteredLobbyId() const;
    };

}

