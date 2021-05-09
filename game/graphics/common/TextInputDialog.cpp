// Поле ввода
#define TEXT_FIELD_TOP_MARGIN 40.0f
#define TEXT_FIELD_LEFT_MARGIN 20.0f
#define TEXT_FIELD_HEIGHT 100.0f
// Текст сообщения
#define MESSAGE_TEXT_LEFT_MARGIN TEXT_FIELD_LEFT_MARGIN
#define MESSAGE_TEXT_AND_TEXT_FIELD_VERTICAL_MARGIN 8.0f
#define MESSAGE_TEXT_FONT_SIZE 27
#define MAX_MESSAGE_TEXT_LINE_LENGTH 70
#define MAX_MESSAGE_TEXT_LINES 3
// Кнопки
#define BUTTONS_LEFT_MARGIN 15.0f
#define BUTTONS_HORIZONTAL_MARGIN 15.0f
#define BUTTONS_BOTTOM_MARGIN 15.0f
#define BUTTONS_WIDTH 250.0f
#define BUTTONS_HEIGHT 75.0f
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 2.0f


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
        float tfTopMargin  = TEXT_FIELD_TOP_MARGIN  * renderScale;
        float tfLeftMargin = TEXT_FIELD_LEFT_MARGIN * renderScale;
        float tfWidth      = width - 2 * tfLeftMargin; // одинаковые отступы слева и справа
        textFieldHeight    = TEXT_FIELD_HEIGHT      * renderScale;
        textFieldY         = y + tfTopMargin;

        auto textField = std::make_shared<TextField>(
                textFieldId, renderScale, window,
                x + tfLeftMargin, textFieldY, tfWidth, textFieldHeight,
                maxInputLen, hintText, initialInput, textFieldListener);

        enqueueAddChild(textField);
    }

    void TextInputDialog::createBasicButtons() {
        const float bLeftMargin   = BUTTONS_LEFT_MARGIN       * renderScale;
        const float bHorizMargin  = BUTTONS_HORIZONTAL_MARGIN * renderScale;
        const float bBottomMargin = BUTTONS_BOTTOM_MARGIN     * renderScale;
        const float bWidth        = BUTTONS_WIDTH             * renderScale;
        const float bHeight       = BUTTONS_HEIGHT            * renderScale;
        buttonsY                  = finalY + height - bHeight - bBottomMargin;

        // Back
        float bX = x + bLeftMargin;
        auto btnNext = std::make_shared<TextButton>(
                btnNextId, renderScale, window,
                L"Далее", bX, buttonsY, bWidth, bHeight, btnNextListener);
        enqueueAddChild(btnNext);

        // Back
        bX += bWidth + bHorizMargin;
        auto btnBack = std::make_shared<TextButton>(
                btnBackId, renderScale, window,
                L"Назад", bX, buttonsY, bWidth, bHeight, btnBackListener);
        enqueueAddChild(btnBack);
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
        this->textFieldId = textFieldId;
        this->textFieldListener = textFieldListener;
        this->maxInputLen = maxInputLen;
        this->hintText = hintText;
        this->initialInput = initialInput;
        this->btnNextId = btnNextId;
        this->btnNextListener = btnNextListener;
        this->btnBackId = btnBackId;
        this->btnBackListener = btnBackListener;

        // Основные составляющие
        createTextField();
        createBasicButtons();

        // Разделительная полоса над кнопками
        float sepMarginX = BUTTONS_LEFT_MARGIN           * renderScale;
        float sepMarginY = BUTTONS_BOTTOM_MARGIN         * renderScale;
        float sepHeight  = BUTTONS_SEPARATOR_LINE_HEIGHT * renderScale + 1; // min 1 px

        sep = std::make_unique<sf::RectangleShape>(
                sf::Vector2f(width - 2 * sepMarginX, sepHeight));

        sep->setFillColor(ColorSet::GUI_BUTTONS_SEPARATOR_LINE);
        sep->setPosition(x + sepMarginX, buttonsY - sepHeight - sepMarginY);

        // Текст сообщения (с поддержкой форматирования)
        std::wstring formattedMsg = StringUtils::wrapByLineLength(
                StringUtils::encodeFormatting(message),
                MAX_MESSAGE_TEXT_LINE_LENGTH, MAX_MESSAGE_TEXT_LINES
        );

        float        msgLeftMargin     = MESSAGE_TEXT_LEFT_MARGIN                    * renderScale;
        float        msgAndTfVerMargin = MESSAGE_TEXT_AND_TEXT_FIELD_VERTICAL_MARGIN * renderScale;
        unsigned int msgFontSize       = MESSAGE_TEXT_FONT_SIZE                      * renderScale;

        auto textField = getChildById(textFieldId);
        msg = std::make_unique<sfe::RichText>(
                *Game::instance().getFontManager()->getRegularFont());

        RenderUtils::enrichText(*msg, formattedMsg);
        msg->setCharacterSize(msgFontSize);
        msg->setPosition(x + msgLeftMargin,
                         textFieldY + textFieldHeight + msgAndTfVerMargin);
    }

    void TextInputDialog::keyPressed(const sf::Event::KeyEvent& event) {
        Dialog::keyPressed(event);

        // Возможность манипулировать диалогом с помощью клавиш ENTER и ESC.
        id_type hotkeyedButtonId = 0L;

        if (event.code == sf::Keyboard::Enter)
            hotkeyedButtonId = btnNextId;
        else if (event.code == sf::Keyboard::Escape)
            hotkeyedButtonId = btnBackId;

        if (hotkeyedButtonId != 0L) {
            auto hotkeyedButton = std::dynamic_pointer_cast<Button>(getChildById(btnNextId));
            hotkeyedButton->click();
        }
    }

    void TextInputDialog::update() {
        Dialog::update();
    }

    void TextInputDialog::draw() {
        Dialog::draw();

        window->draw(*sep);
        window->draw(*msg);
    }

}
