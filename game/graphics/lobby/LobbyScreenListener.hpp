#pragma once


#include <packets.pb.h>
#include "../common/DialogListener.hpp"
#include "../common/ButtonListener.hpp"
#include "../../lobby/Lobby.hpp"
#include "../play/PlayScreen.hpp"

namespace awd::game {

    class LobbyScreenListener :
            public DialogListener,
            public ButtonListener {
    private:
        std::shared_ptr<PlayScreen> playScreen = nullptr;

        //////////////////////////////////////////////////////////////////////////////////////////////
        //   Утилити-методы
        //////////////////////////////////////////////////////////////////////////////////////////////

        static void beginLeaveLobby(Drawable* lobbyDrawable);

        static void beginNewGame(Drawable* lobbyDrawable);

        //////////////////////////////////////////////////////////////////////////////////////////////
        //   Обработчики событий
        //////////////////////////////////////////////////////////////////////////////////////////////

        static void leaveLobbyClicked(Drawable* lobbyDrawable);

        static void newGameClicked(Drawable* lobbyDrawable);

        static void errorOkClicked(Drawable* dialog);

    public:
        static void finishLeaveLobby(Drawable* lobbyDrawable,
                                     const std::shared_ptr<net::LeaveLobbyResponse>& response);

        static void kickedFromLobby(Drawable* lobbyDrawable,
                                    const std::shared_ptr<net::KickedFromLobby>& kick);

        static void finishBeginPlayState(Drawable* lobbyDrawable,
                                         const std::shared_ptr<net::BeginPlayStateResponse>& response);

        static void initialUpdateDimension(Drawable* lobbyDrawable,
                                           const std::shared_ptr<net::UpdateDimensionCommand>& command);

        static void joinWorld(Drawable* lobbyDrawable,
                              const std::shared_ptr<net::JoinWorldCommand>& command);

        void dialogOpened(Drawable* parentScreen, id_type dialogId) override;

        void dialogClosed(Drawable* parentScreen, id_type dialogId) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId) override;
    };

}

