// Поле ввода
#define TEXT_FIELD_TOP_MARGIN 50
#define TEXT_FIELD_LEFT_MARGIN 15
#define TEXT_FIELD_HEIGHT 100
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

    void TextInputDialog::createTextField() {
        unsigned int tfTopMargin  = TEXT_FIELD_TOP_MARGIN  * renderScale;
        unsigned int tfLeftMargin = TEXT_FIELD_LEFT_MARGIN * renderScale;
        unsigned int tfWidth      = width - 2 * tfLeftMargin; // одинаковые отступы слева и справа
        unsigned int tfHeight     = TEXT_FIELD_HEIGHT      * renderScale;

        auto textField = std::make_shared<TextField>(
                textFieldId, renderScale, window,
                x + tfLeftMargin, y + tfTopMargin, tfWidth, tfHeight,
                maxInputLen, hintText, textFieldListener);
        addChild(textField);
    }

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

    TextInputDialogListener::TextInputDialogListener(Drawable* textInputDialogParent,
                                                     id_type btnNextId) {
        this->textInputDialogParent = textInputDialogParent;
        this->btnNextId = btnNextId;
    }

    void TextInputDialogListener::contentsChanged(Drawable* textFieldParent, id_type textFieldId,
                                                  const std::wstring& newContents) {
        auto buttonNext = std::static_pointer_cast
                <Button<std::wstring>>(textInputDialogParent->getChildByIdRecursively(btnNextId));

        buttonNext->setPayload(newContents);
    }

    TextInputDialog::TextInputDialog(id_type id,
                                     float renderScale,
                                     const std::shared_ptr<sf::RenderWindow>& window,
                                     const std::shared_ptr<DialogListener>& dialogListener,
                                     Drawable* textInputDialogParent,
                                     id_type textFieldId,
                                     unsigned int maxInputLen,
                                     const std::wstring& hintText,
                                     id_type btnNextId,
                                     const std::shared_ptr<ButtonListener<std::wstring>>& btnNextListener,
                                     id_type btnBackId,
                                     const std::shared_ptr<ButtonListener<NoPayload>>& btnBackListener)
                                     : Dialog(id, renderScale, window, dialogListener) {
        this->textFieldId = textFieldId;
        this->maxInputLen = maxInputLen;
        this->hintText = hintText;
        this->textFieldListener = std::make_shared
                <TextInputDialogListener>(textInputDialogParent, btnNextId);
        this->btnNextId = btnNextId;
        this->btnNextListener = btnNextListener;
        this->btnBackId = btnBackId;
        this->btnBackListener = btnBackListener;

        createTextField();
        createBasicButtons();
    }

    void TextInputDialog::update() {
        Dialog::update();
    }

    void TextInputDialog::draw() {
        Dialog::draw();
    }

}
