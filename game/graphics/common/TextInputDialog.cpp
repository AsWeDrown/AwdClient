#include "TextInputDialog.hpp"
#include "Screen.hpp"

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
        auto* parentScreen = (Screen*) dialog->parent;
        dialog->nextClicked(parentScreen, L"TODO");
    }

    void TextInputDialog::localBackClicked(Drawable* buttonOwner) {
        printf("local back\n");
        auto* dialog = (TextInputDialog*) buttonOwner;
        auto* parentScreen = (Screen*) dialog->parent;
        parentScreen->closeCurrentDialog();
    }

    void TextInputDialog::createBasicButtons() {
        addButton(std::make_shared<TextButton>(renderScale, window, L"Cancel",
                                                   x + 50, y + 50, 180, 60, localBackClicked));
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PROTECTED
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void TextInputDialog::createButtons() {
        // Дочерние классы должны использовать этот метод для добавления своих кнопок.
        // При этом они обязаны вызвать этот метод самостоятельно, внутри своего конструктора.
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TextInputDialog::TextInputDialog(float renderScale,
                                     const std::shared_ptr<sf::RenderWindow>& window,
                                     void (*nextClicked)(Drawable* buttonOwner, const std::wstring& userInput))
                                     : Dialog(renderScale, window) {
        this->renderScale = renderScale;
        this->window = window;
        this->nextClicked = nextClicked;

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
