#pragma once


#include <packets.pb.h>
#include "../common/DialogListener.hpp"
#include "../common/ButtonListener.hpp"
#include "../../lobby/Lobby.hpp"

namespace awd::game {

    class LobbyScreenListener :
            public DialogListener,
            public ButtonListener {
    private:
        //////////////////////////////////////////////////////////////////////////////////////////////
        //   Утилити-методы
        //////////////////////////////////////////////////////////////////////////////////////////////

        static void beginLeaveLobby(Drawable* lobbyDrawable,
                                    const std::shared_ptr<Lobby>& currentLobby);

        //////////////////////////////////////////////////////////////////////////////////////////////
        //   Обработчики событий
        //////////////////////////////////////////////////////////////////////////////////////////////

        static void leaveLobbyClicked(Drawable* lobbyDrawable);

        static void errorOkClicked(Drawable* dialog);

    public:
        static void finishLeaveLobby(Drawable* lobbyDrawable,
                                     const std::shared_ptr<net::LeaveLobbyResponse>& response);

        static void kickedFromLobby(Drawable* lobbyDrawable,
                                    const std::shared_ptr<net::KickedFromLobby>& kick);

        void dialogOpened(Drawable* parentScreen, id_type dialogId) override;

        void dialogClosed(Drawable* parentScreen, id_type dialogId) override;

        void buttonClicked(Drawable* buttonParent, id_type buttonId) override;
    };

}

