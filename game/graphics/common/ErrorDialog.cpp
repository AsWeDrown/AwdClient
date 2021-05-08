// Текст сообщения
#define MESSAGE_TEXT_LEFT_MARGIN 20.0f
#define MESSAGE_TEXT_TOP_MARGIN 20.0f
#define MESSAGE_TEXT_FONT_SIZE 27
#define MAX_MESSAGE_TEXT_LINE_LENGTH 66
#define MAX_MESSAGE_TEXT_LINES 9
// Кнопка "ОК"
#define BUTTON_OK_LEFT_MARGIN 15.0f
#define BUTTON_OK_BOTTOM_MARGIN 15.0f
#define BUTTON_OK_WIDTH 250.0f
#define BUTTON_OK_HEIGHT 75.0f
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 2.0f


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
        // Кнопка "ОК".
        float bLeftMargin   = BUTTON_OK_LEFT_MARGIN       * renderScale;
        float bBottomMargin = BUTTON_OK_BOTTOM_MARGIN     * renderScale;
        float bWidth        = BUTTON_OK_WIDTH             * renderScale;
        float bHeight       = BUTTON_OK_HEIGHT            * renderScale;
        float bX            = x + bLeftMargin;
        float bY            = finalY + height - bHeight - bBottomMargin;

        auto btnOk = std::make_shared<TextButton>(
                btnOkId, renderScale, window,
                L"ОК", bX, bY, bWidth, bHeight, btnOkListener);
        addChild(btnOk);

        // Разделительная линия над кнопками.
        float sepMarginX = BUTTON_OK_LEFT_MARGIN         * renderScale;
        float sepMarginY = BUTTON_OK_BOTTOM_MARGIN       * renderScale;
        float sepHeight  = BUTTONS_SEPARATOR_LINE_HEIGHT * renderScale + 1.0f; // min 1 px
        float btnOkY     = btnOk->getY();

        sepLine = std::make_unique<sf::RectangleShape>(
                sf::Vector2f(width - 2 * sepMarginX, sepHeight));

        sepLine->setFillColor(ColorSet::GUI_BUTTONS_SEPARATOR_LINE);
        sepLine->setPosition(x + sepMarginX, btnOkY - sepHeight - sepMarginY);

        // Текст сообщения (с поддержкой форматирования)
        std::wstring formattedMsg = StringUtils::wrapByLineLength(
                StringUtils::encodeFormatting(message),
                MAX_MESSAGE_TEXT_LINE_LENGTH, MAX_MESSAGE_TEXT_LINES
        );

        float        msgLeftMargin = MESSAGE_TEXT_LEFT_MARGIN * renderScale;
        float        msgTopMargin  = MESSAGE_TEXT_TOP_MARGIN  * renderScale;
        unsigned int msgFontSize   = MESSAGE_TEXT_FONT_SIZE   * renderScale;

        text = std::make_unique<sfe::RichText>(
                *Game::instance().getFontManager()->getRegularFont());

        RenderUtils::enrichText(*text, formattedMsg);
        text->setCharacterSize(msgFontSize);
        text->setPosition(x + msgLeftMargin, finalY + msgTopMargin);
    }

    void ErrorDialog::update() {
        Dialog::update();
    }

    void ErrorDialog::draw() {
        Dialog::draw();

        window->draw(*sepLine);
        window->draw(*text);
    }

}
