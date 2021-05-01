// Кнопки
#define BUTTONS_LEFT_MARGIN 15
#define BUTTONS_HORIZONTAL_MARGIN 15
#define BUTTONS_BOTTOM_MARGIN 15
#define BUTTONS_WIDTH 250
#define BUTTONS_HEIGHT 75


#include "TextInputDialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//    void TextInputDialog::localNextClicked(Drawable* buttonOwner) {
//        // TODO - передавать текст из textField
//        // TODO - передавать текст из textField
//        // TODO - передавать текст из textField
//        auto* dialog = (TextInputDialog*) buttonOwner;
//        auto* parentScreen = (Drawable*) dialog->parent;
//        dialog->nextClicked(parentScreen, L"TODO");
//    }
//
//    void TextInputDialog::localBackClicked(Drawable* buttonOwner) {
//        auto* dialog = (TextInputDialog*) buttonOwner;
//        auto* parentScreen = (Drawable*) dialog->parent;
//        dialog->backClicked(parentScreen);
//    }

    void TextInputDialog::createBasicButtons() {
        const unsigned int bLeftMargin   = BUTTONS_LEFT_MARGIN       * renderScale;
        const unsigned int bHorizMargin  = BUTTONS_HORIZONTAL_MARGIN * renderScale;
        const unsigned int bBottomMargin = BUTTONS_BOTTOM_MARGIN     * renderScale;
        const unsigned int bWidth        = BUTTONS_WIDTH             * renderScale;
        const unsigned int bHeight       = BUTTONS_HEIGHT            * renderScale;
        const unsigned int bY            = finalY + height - bHeight - bBottomMargin;

        // Back
        unsigned int bX = x + bLeftMargin;
        auto btnNext = std::make_shared<TextButton<std::wstring>>(
                btnNextId, renderScale, window,
                L"Далее", bX, bY, bWidth, bHeight, btnNextListener);
        addChild(btnNext);

        // Back
        bX += bWidth + bHorizMargin;
        auto btnBack = std::make_shared<TextButton<NoPayload>>(
                btnBackId, renderScale, window,
                L"Назад", bX, bY, bWidth, bHeight, btnBackListener);
        addChild(btnBack);
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TextInputDialog::TextInputDialog(id_type id,
                                     float renderScale,
                                     const std::shared_ptr<sf::RenderWindow>& window,
                                     const std::shared_ptr<DialogListener>& dialogListener,
                                     id_type btnNextId,
                                     const std::shared_ptr<ButtonListener<std::wstring>>& btnNextListener,
                                     id_type btnBackId,
                                     const std::shared_ptr<ButtonListener<NoPayload>>& btnBackListener)
                                     : Dialog(id, renderScale, window, dialogListener) {
        this->btnNextId = btnNextId;
        this->btnNextListener = btnNextListener;
        this->btnBackId = btnBackId;
        this->btnBackListener = btnBackListener;

        // Мы не можем вызвать дочерний метод createButtons в родительском конструкторе,
        // так что ограничиваемся здесь лишь созданием базовых кнопок (next, back).
        createBasicButtons();
    }

    void TextInputDialog::update() {
        Dialog::update();
    }

    void TextInputDialog::draw() {
        Dialog::draw();
    }

}
