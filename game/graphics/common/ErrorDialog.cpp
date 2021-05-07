// Текст сообщения
#define MESSAGE_TEXT_LEFT_MARGIN 20
#define MESSAGE_TEXT_TOP_MARGIN 20
#define MESSAGE_TEXT_FONT_SIZE 27
#define MAX_MESSAGE_TEXT_LINE_LENGTH 66
#define MAX_MESSAGE_TEXT_LINES 9
// Кнопка "ОК"
#define BUTTON_OK_LEFT_MARGIN 15
#define BUTTON_OK_BOTTOM_MARGIN 15
#define BUTTON_OK_WIDTH 250
#define BUTTON_OK_HEIGHT 75
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 2


#include "ErrorDialog.hpp"
#include "../../util/StringUtils.hpp"
#include "../../Game.hpp"
#include "../../util/RenderUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    ErrorDialog::ErrorDialog(id_type id,
                             float renderScale,
                             const std::shared_ptr<sf::RenderWindow>& window,
                             const std::shared_ptr<DialogListener>& dialogListener,
                             const std::wstring& message,
                             id_type btnOkId,
                             const std::shared_ptr<ButtonListener>& btnOkListener)
                             : Dialog(id, renderScale, window, dialogListener) {
        this->message = StringUtils::wrapByLineLength(
                StringUtils::encodeFormatting(message),
                MAX_MESSAGE_TEXT_LINE_LENGTH, MAX_MESSAGE_TEXT_LINES);

        this->btnOkId = btnOkId;

        // Создаём кнопку "ОК".
        unsigned int bLeftMargin   = BUTTON_OK_LEFT_MARGIN       * renderScale;
        unsigned int bBottomMargin = BUTTON_OK_BOTTOM_MARGIN     * renderScale;
        unsigned int bWidth        = BUTTON_OK_WIDTH             * renderScale;
        unsigned int bHeight       = BUTTON_OK_HEIGHT            * renderScale;
        unsigned int bX            = x + bLeftMargin;
        unsigned int bY            = finalY + height - bHeight - bBottomMargin;

        auto btnOk = std::make_shared<TextButton>(
                btnOkId, renderScale, window,
                L"ОК", bX, bY, bWidth, bHeight, btnOkListener);
        addChild(btnOk);
    }

    void ErrorDialog::update() {
        Dialog::update();
    }

    void ErrorDialog::draw() {
        Dialog::draw();

        // Разделительная полоса над кнопками
        unsigned int sepMarginX = BUTTON_OK_LEFT_MARGIN         * renderScale;
        unsigned int sepMarginY = BUTTON_OK_BOTTOM_MARGIN       * renderScale;
        unsigned int sepHeight  = BUTTONS_SEPARATOR_LINE_HEIGHT * renderScale + 1; // min 1 px
        unsigned int btnOkY     = getChildById(btnOkId)->getY();

        sf::RectangleShape sep(sf::Vector2f(width - 2 * sepMarginX, sepHeight));
        sep.setFillColor(ColorSet::GUI_BUTTONS_SEPARATOR_LINE);
        sep.setPosition(x + sepMarginX, btnOkY - sepHeight - sepMarginY);
        window->draw(sep);

        // Текст сообщения (с поддержкой форматирования).
        unsigned int msgLeftMargin = MESSAGE_TEXT_LEFT_MARGIN * renderScale;
        unsigned int msgTopMargin  = MESSAGE_TEXT_TOP_MARGIN  * renderScale;
        unsigned int msgFontSize   = MESSAGE_TEXT_FONT_SIZE   * renderScale;

        sfe::RichText msg(*Game::instance().getFontManager()->getRegularFont());
        RenderUtils::richText(msg, message);

        // Оставшиеся мелочи.
        msg.setCharacterSize(msgFontSize);
        msg.setPosition(x + msgLeftMargin, y + msgTopMargin);
        window->draw(msg);
    }

}
