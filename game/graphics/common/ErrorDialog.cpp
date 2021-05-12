// Текст сообщения
#define MESSAGE_TEXT_LEFT_MARGIN 20.0f
#define MESSAGE_TEXT_TOP_MARGIN 20.0f
#define MESSAGE_TEXT_FONT_SIZE 27
#define MAX_MESSAGE_TEXT_LINE_LENGTH 66
#define MAX_MESSAGE_TEXT_LINES 9
// Кнопка "ОК"
#define BUTTON_OK_LEFT_MARGIN 15.0f
#define BUTTON_OK_BOTTOM_MARGIN 15.0f
#define BUTTON_OK_WIDTH 200.0f
#define BUTTON_OK_HEIGHT 60.0f
#define BUTTON_OK_FONT_SIZE 40
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 2.0f


#include "ErrorDialog.hpp"
#include "../../util/StringUtils.hpp"
#include "../../Game.hpp"
#include "../../util/RenderUtils.hpp"
#include "TextButton.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    ErrorDialog::ErrorDialog(id_type id,
                             const std::shared_ptr<DialogListener>& dialogListener,
                             const std::wstring& message,
                             id_type btnOkId,
                             const std::shared_ptr<ButtonListener>& btnOkListener)
                             : Dialog(id, dialogListener) {
        this->btnOkId = btnOkId;

        // Кнопка "ОК".
        float    bLeftMargin   = BUTTON_OK_LEFT_MARGIN       * renderScale;
        float    bBottomMargin = BUTTON_OK_BOTTOM_MARGIN     * renderScale;
        float    bWidth        = BUTTON_OK_WIDTH             * renderScale;
        float    bHeight       = BUTTON_OK_HEIGHT            * renderScale;
        uint32_t bFontSize     = BUTTON_OK_FONT_SIZE         * renderScale;
        float    bX            = x + bLeftMargin;
        float    bY            = finalY + height - bHeight - bBottomMargin;

        auto btnOk = std::make_shared<TextButton>(
                btnOkId, L"ОК",
                bX, bY, bWidth, bHeight, bFontSize, btnOkListener);
        enqueueAddChild(btnOk);

        // Разделительная линия над кнопками.
        float sepMarginX = BUTTON_OK_LEFT_MARGIN         * renderScale;
        float sepMarginY = BUTTON_OK_BOTTOM_MARGIN       * renderScale;
        float sepHeight  = BUTTONS_SEPARATOR_LINE_HEIGHT * renderScale + 1.0f; // min 1 px
        float btnOkY     = btnOk->getY();

        sepLine = std::make_unique<sf::RectangleShape>(
                sf::Vector2f(width - 2.0f * sepMarginX, sepHeight));

        sepLine->setFillColor(ColorSet::GUI_BUTTONS_SEPARATOR_LINE);
        sepLine->setPosition(x + sepMarginX, btnOkY - sepHeight - sepMarginY);

        // Текст сообщения (с поддержкой форматирования)
        std::wstring formattedMsg = StringUtils::wrapByLineLength(
                StringUtils::encodeFormatting(message),
                MAX_MESSAGE_TEXT_LINE_LENGTH, MAX_MESSAGE_TEXT_LINES
        );

        float    msgLeftMargin = MESSAGE_TEXT_LEFT_MARGIN * renderScale;
        float    msgTopMargin  = MESSAGE_TEXT_TOP_MARGIN  * renderScale;
        uint32_t msgFontSize   = MESSAGE_TEXT_FONT_SIZE   * renderScale;

        text = std::make_unique<sfe::RichText>(
                *Game::instance().getFonts()->regularFont);

        RenderUtils::enrichText(*text, formattedMsg);
        text->setCharacterSize(msgFontSize);
        text->setPosition(x + msgLeftMargin, finalY + msgTopMargin);
    }

    void ErrorDialog::keyPressed(const sf::Event::KeyEvent& event) {
        if (state != DialogState::APPEARED)
            return;

        Dialog::keyPressed(event);

        // Возможность манипулировать диалогом с помощью клавиш ENTER и ESC.
        if (event.code == sf::Keyboard::Enter || event.code == sf::Keyboard::Escape) {
            auto btnOk = std::dynamic_pointer_cast<Button>(getChildById(btnOkId));
            btnOk->click();
        }
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
