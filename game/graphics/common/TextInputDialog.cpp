// ID
#define ID_BUTTON_NEXT 1
#define ID_BUTTON_CANCEL 2


#include "TextInputDialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void TextInputDialog::localNextClicked(Drawable* buttonOwner) {
        // TODO - передавать текст из textField
        // TODO - передавать текст из textField
        // TODO - передавать текст из textField
        auto* dialog = (TextInputDialog*) buttonOwner;
        auto* parentScreen = (Drawable*) dialog->parent;
        dialog->nextClicked(parentScreen, L"TODO");
    }

    void TextInputDialog::localBackClicked(Drawable* buttonOwner) {
        auto* dialog = (TextInputDialog*) buttonOwner;
        auto* parentScreen = (Drawable*) dialog->parent;
        dialog->backClicked(parentScreen);
    }

    void TextInputDialog::createBasicButtons() {
        addChild(std::make_shared<TextButton>(ID_BUTTON_CANCEL, renderScale, window, L"Cancel",
                                                   x + 50, y + 50, 180, 60, localBackClicked));
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TextInputDialog::TextInputDialog(int id,
                                     float renderScale,
                                     const std::shared_ptr<sf::RenderWindow>& window,
                                     void (*dialogOpened)(Drawable*, int),
                                     void (*dialogClosed)(Drawable*, int),
                                     void (*nextClicked)(Drawable*, const std::wstring&),
                                     void (*backClicked)(Drawable*))
                                     : Dialog(id, renderScale, window, dialogOpened, dialogClosed) {
        this->renderScale = renderScale;
        this->window = window;
        this->nextClicked = nextClicked;
        this->backClicked = backClicked;

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
