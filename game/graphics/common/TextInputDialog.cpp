// Поле ввода
#define TEXT_FIELD_TOP_MARGIN 40
#define TEXT_FIELD_LEFT_MARGIN 20
#define TEXT_FIELD_HEIGHT 100
// Текст сообщения
#define MESSAGE_TEXT_LEFT_MARGIN TEXT_FIELD_LEFT_MARGIN
#define MESSAGE_TEXT_AND_TEXT_FIELD_VERTICAL_MARGIN 8
#define MESSAGE_TEXT_FONT_SIZE 27
#define MAX_MESSAGE_TEXT_LINE_LENGTH 70
#define MAX_MESSAGE_TEXT_LINES 4
// Кнопки
#define BUTTONS_LEFT_MARGIN 15
#define BUTTONS_HORIZONTAL_MARGIN 15
#define BUTTONS_BOTTOM_MARGIN 15
#define BUTTONS_WIDTH 250
#define BUTTONS_HEIGHT 75


#include <sstream>
#include "TextInputDialog.hpp"
#include "ColorSet.hpp"
#include "../../util/StringUtils.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void TextInputDialog::applyFormatting(sfe::RichText& msg, wchar_t code) {
        switch (code) {
            // Сброс стиля и цвета
            case L'r':
            case L'R':
                msg << sf::Text::Regular << ColorSet::MESSAGE_TEXT_WHITE;
                break;

            // Стили
            case L'l':
            case L'L':
                msg << sf::Text::Bold;
                break;

            case L'o':
            case L'O':
                msg << sf::Text::Italic;
                break;

            case L'n':
            case L'N':
                msg << sf::Text::Underlined;
                break;

            case L'm':
            case L'M':
                msg << sf::Text::StrikeThrough;
                break;

            // Цвета
            case L'f':
            case L'F':
                msg << ColorSet::MESSAGE_TEXT_WHITE;
                break;

            case L'7':
                msg << ColorSet::MESSAGE_TEXT_GRAY;
                break;

            case L'0':
                msg << ColorSet::MESSAGE_TEXT_BLACK;
                break;

            case L'e':
            case L'E':
                msg << ColorSet::MESSAGE_TEXT_YELLOW;
                break;

            case L'6':
                msg << ColorSet::MESSAGE_TEXT_GOLD;
                break;

            case L'a':
            case L'A':
                msg << ColorSet::MESSAGE_TEXT_GREEN;
                break;

            case L'c':
            case L'C':
                msg << ColorSet::MESSAGE_TEXT_RED;
                break;

            case L'b':
            case L'B':
                msg << ColorSet::MESSAGE_TEXT_AQUA;
                break;

            case L'9':
                msg << ColorSet::MESSAGE_TEXT_BLUE;
                break;

            case L'd':
            case L'D':
                msg << ColorSet::MESSAGE_TEXT_MAGENTA;
                break;

            // Неизвестный код - пропускаем
            default:
                break;
        }
    }

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

        // Текст сообщения (с поддержкой форматирования).
        unsigned int msgLeftMargin     = MESSAGE_TEXT_LEFT_MARGIN                    * renderScale;
        unsigned int msgAndTfVerMargin = MESSAGE_TEXT_AND_TEXT_FIELD_VERTICAL_MARGIN * renderScale;
        unsigned int msgFontSize       = MESSAGE_TEXT_FONT_SIZE                      * renderScale;

        auto textField = getChildById(textFieldId);
        sfe::RichText msg(*Game::instance().getFontManager()->getRegularFont());

        // Форматирование.
        std::wstring::size_type offset = 0; // отступ слева от начала текста при выводе очередной части сообщения
        applyFormatting(msg, L'r'); // сброс цвета на белый при каждом выводе

        for (std::wstring::size_type i = 0; i < message.length() - 1; i++) {
            wchar_t code = message[i + 1];

            if (message[i] == '&' && code != '&') { // нашли цветовой код
                // Выводим текст слева от нового цветового кода.
                std::wstring prev = message.substr(offset, i - offset);
                msg << prev;

                applyFormatting(msg, code); // устанавливаем цвет для вывода текста справа от нового цветового кода
                offset = i + 2; // пропускаем сам цветовой код при следующем выводе
                i++; // перескакиваем через следующий символ (цветовой код после '&')
            }
        }

        std::wstring remainder = message.substr(offset, message.length());  // оставшийся текст (если есть)
        msg << remainder; // ^
        applyFormatting(msg, L'r'); // сброс цвета на белый после каждого вывода

        // Оставшиеся мелочи.
        msg.setCharacterSize(msgFontSize);
        msg.setPosition(x + msgLeftMargin, textField->getY() + textField->getHeight() + msgAndTfVerMargin);
        window->draw(msg);
    }

}
