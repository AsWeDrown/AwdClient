// Поле ввода
#define TEXT_FIELD_TOP_MARGIN 40
#define TEXT_FIELD_LEFT_MARGIN 20
#define TEXT_FIELD_HEIGHT 100
// Текст сообщения
#define MESSAGE_TEXT_LEFT_MARGIN TEXT_FIELD_LEFT_MARGIN
#define MESSAGE_TEXT_AND_TEXT_FIELD_VERTICAL_MARGIN 8
#define MESSAGE_TEXT_FONT_SIZE 27
#define MAX_MESSAGE_TEXT_LINE_LENGTH 70
#define MAX_MESSAGE_TEXT_LINES 3
// Кнопки
#define BUTTONS_LEFT_MARGIN 15
#define BUTTONS_HORIZONTAL_MARGIN 15
#define BUTTONS_BOTTOM_MARGIN 15
#define BUTTONS_WIDTH 250
#define BUTTONS_HEIGHT 75
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 2


#include "TextInputDialog.hpp"
#include "../../util/StringUtils.hpp"
#include "../../Game.hpp"
#include "../../util/RenderUtils.hpp"

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
                maxInputLen, hintText, initialInput, textFieldListener);

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
        auto btnNext = std::make_shared<TextButton>(
                btnNextId, renderScale, window,
                L"Далее", bX, bY, bWidth, bHeight, btnNextListener);
        addChild(btnNext);

        // Back
        bX += bWidth + bHorizMargin;
        auto btnBack = std::make_shared<TextButton>(
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
                                     const std::wstring& message,
                                     id_type textFieldId,
                                     const std::shared_ptr<TextFieldListener>& textFieldListener,
                                     unsigned int maxInputLen,
                                     const std::wstring& hintText,
                                     const std::wstring& initialInput,
                                     id_type btnNextId,
                                     const std::shared_ptr<ButtonListener>& btnNextListener,
                                     id_type btnBackId,
                                     const std::shared_ptr<ButtonListener>& btnBackListener)
                                     : Dialog(id, renderScale, window, dialogListener) {
        this->message = StringUtils::wrapByLineLength(
                StringUtils::encodeFormatting(message),
                MAX_MESSAGE_TEXT_LINE_LENGTH, MAX_MESSAGE_TEXT_LINES);
        this->textFieldId = textFieldId;
        this->textFieldListener = textFieldListener;
        this->maxInputLen = maxInputLen;
        this->hintText = hintText;
        this->initialInput = initialInput;
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

        // Разделительная полоса над кнопками
        unsigned int sepMarginX = BUTTONS_LEFT_MARGIN           * renderScale;
        unsigned int sepMarginY = BUTTONS_BOTTOM_MARGIN         * renderScale;
        unsigned int sepHeight  = BUTTONS_SEPARATOR_LINE_HEIGHT * renderScale + 1; // min 1 px
        unsigned int btnNextY   = getChildById(btnNextId)->getY();

        sf::RectangleShape sep(sf::Vector2f(width - 2 * sepMarginX, sepHeight));
        sep.setFillColor(ColorSet::GUI_BUTTONS_SEPARATOR_LINE);
        sep.setPosition(x + sepMarginX, btnNextY - sepHeight - sepMarginY);
        window->draw(sep);

        // Текст сообщения (с поддержкой форматирования).
        unsigned int msgLeftMargin     = MESSAGE_TEXT_LEFT_MARGIN                    * renderScale;
        unsigned int msgAndTfVerMargin = MESSAGE_TEXT_AND_TEXT_FIELD_VERTICAL_MARGIN * renderScale;
        unsigned int msgFontSize       = MESSAGE_TEXT_FONT_SIZE                      * renderScale;

        auto textField = getChildById(textFieldId);
        sfe::RichText msg(*Game::instance().getFontManager()->getRegularFont());
        RenderUtils::richText(msg, message);

        // Оставшиеся мелочи.
        msg.setCharacterSize(msgFontSize);
        msg.setPosition(x + msgLeftMargin, textField->getY() + textField->getHeight() + msgAndTfVerMargin);
        window->draw(msg);
    }

}
