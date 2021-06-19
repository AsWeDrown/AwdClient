#include <iostream>
#include "PlayScreenListener.hpp"
#include "PlayScreen.hpp"
#include "../../Game.hpp"
#include "../mainmenu/MainMenuScreen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Обработчики событий
    //////////////////////////////////////////////////////////////////////////////////////////////

    void PlayScreenListener::resumeClicked(Drawable* playDrawable) {
        auto* playScreen = (PlayScreen*) playDrawable;
        playScreen->closeCurrentDialog();
    }

    void PlayScreenListener::quitClicked(Drawable* playDrawable) {
        Game::instance().returnToMainMenu();
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void PlayScreenListener::dialogOpened(Drawable* parentScreen, id_type dialogId) {}

    void PlayScreenListener::dialogClosed(Drawable* parentScreen, id_type dialogId) {
        auto* playScreen = (PlayScreen*) parentScreen;
        playScreen->dialogClosed(); // для удаления диалога из списка потомков
    }

    void PlayScreenListener::buttonClicked(Drawable* buttonParent, id_type buttonId) {
        switch (buttonId) {
            // PlayScreen
            case ID_SCREEN_PLAY_DIALOG_ESC_BUTTON_RESUME:
                resumeClicked(buttonParent);
                break;

            case ID_SCREEN_PLAY_DIALOG_ESC_BUTTON_QUIT:
                quitClicked(buttonParent);
                break;

            // ???
            default:
                std::wcerr << L"Unhandled button click: parent=" << buttonParent->getId()
                           << L", buttonId=" << buttonId << std::endl;

                break;
        }
    }

}
