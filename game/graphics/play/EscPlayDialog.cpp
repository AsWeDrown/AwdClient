// Затемнение фона
#define DARKEN_RECT_ALPHA 100
// Кнопки
#define BUTTONS_VERTICAL_MARGIN 25.0f
#define BUTTONS_WIDTH 500.0f
#define BUTTONS_HEIGHT 60.0f
#define BUTTONS_FONT_SIZE 40


#include "EscPlayDialog.hpp"
#include "../common/TextButton.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    EscPlayDialog::EscPlayDialog(const std::shared_ptr<DialogListener>& dialogListener,
                                 const std::shared_ptr<ButtonListener>& btnResumeListener,
                                 const std::shared_ptr<ButtonListener>& btnQuitListener)
                                 : Dialog(ID_SCREEN_PLAY_DIALOG_ESC, dialogListener) {
        sf::Vector2f viewCenter = window->getView().getCenter();
        sf::Vector2f viewSize = window->getView().getSize();
        float viewX = viewCenter.x - viewSize.x / 2.0f;
        float viewY = viewCenter.y - viewSize.y / 2.0f;

        // Затемнение фона
        rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
        rect->setFillColor(sf::Color(0, 0, 0, DARKEN_RECT_ALPHA));
        rect->setPosition(viewX, viewY);

        // Кнопки
        uint32_t    bFontSize  = BUTTONS_FONT_SIZE         * renderScale;
        const float bVerMargin = BUTTONS_VERTICAL_MARGIN   * renderScale;
        const float bWidth     = BUTTONS_WIDTH             * renderScale;
        const float bHeight    = BUTTONS_HEIGHT            * renderScale;
        const float bX         = viewX + viewSize.x / 2.0f - bWidth / 2.0f;

        // Resume
        float bY = viewY + viewSize.y / 2.0f + bVerMargin;
        auto btnResume = std::make_shared<TextButton>(
                ID_SCREEN_PLAY_DIALOG_ESC_BUTTON_RESUME, L"Вернуться к игре",
                bX, bY, bWidth, bHeight, bFontSize, btnResumeListener);
        enqueueAddChild(btnResume);

        // Quit
        bY += bVerMargin + bHeight;
        auto btnQuit = std::make_shared<TextButton>(
                ID_SCREEN_PLAY_DIALOG_ESC_BUTTON_QUIT, L"Выйти из игры",
                bX, bY, bWidth, bHeight, bFontSize, btnQuitListener);
        enqueueAddChild(btnQuit);
    }

    void EscPlayDialog::update() {
        Dialog::update();

        sf::Vector2f viewCenter = window->getView().getCenter();
        sf::Vector2f viewSize = window->getView().getSize();
        float viewX = viewCenter.x - viewSize.x / 2.0f;
        float viewY = viewCenter.y - viewSize.y / 2.0f;

        // Затемнениею
        rect->setPosition(viewX, viewY);

        // Окно диалога.
        // Такой вот временный костыль, т.к. обычный Dialog не работает корректно при использовании View.
        float realDialogX = viewSize.x / 2.0f - width  / 2.0f;
        float realDialogY = viewSize.y / 2.0f - height / 2.0f;

        fill->setPosition(realDialogX, realDialogY);
    }

    void EscPlayDialog::draw() {
        window->draw(*rect);
        Dialog::draw();
    }

}
