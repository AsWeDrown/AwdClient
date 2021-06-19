// Фон
#define RECT_ALPHA 150
#define RECT_WIDTH 350.0f
#define RECT_HEIGHT 225.0f
#define CHAT_TOP_MARGIN 600.0f
// Текст
#define TEXT_FONT_SIZE 27
#define TEXT_LEFT_MARGIN 10.0f
#define TEXT_TOP_MARGIN 10.0f
#define MAX_LINE_LENGTH 28
#define MAX_LINES 10


#include "Chat.hpp"
#include "../../util/StringUtils.hpp"
#include "../../Game.hpp"
#include "../../util/RenderUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Chat::Chat() : Drawable(ID_SCREEN_PLAY_CHAT) {
        // Фон
        width  = RECT_WIDTH  * renderScale;
        height = RECT_HEIGHT * renderScale;

        rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
        rect->setFillColor(sf::Color(0, 0, 0, RECT_ALPHA));

        // Текст
        uint32_t textFontSize = TEXT_FONT_SIZE * renderScale;

        text = std::make_unique<sfe::RichText>(*Game::instance().getFonts()->regularFont);
        text->setCharacterSize(textFontSize);
    }

    void Chat::update() {
        Drawable::update();

        sf::Vector2f viewCenter = window->getView().getCenter();
        sf::Vector2f viewSize = window->getView().getSize();
        float viewX = viewCenter.x - viewSize.x / 2.0f;
        float viewY = viewCenter.y - viewSize.y / 2.0f;

        // Фон
        float chatTopMargin = CHAT_TOP_MARGIN * renderScale;
        rect->setPosition(viewX, viewY + chatTopMargin);

        // Текст
        float textLeftMargin = TEXT_LEFT_MARGIN * renderScale;
        float textTopMargin  = TEXT_TOP_MARGIN  * renderScale;
        text->setPosition(viewX + textLeftMargin, viewY + chatTopMargin + textTopMargin);
    }

    void Chat::draw() {
        Drawable::draw();

        // FIXME: см. коммент в StringUtils#getUtf8WideString
//        window->draw(*rect);
//        window->draw(*text);
    }

    void Chat::addChatMessage(const std::wstring& message) {
        // Сначала просто строим новый текст, содержащий все сообщения
        // в чате, без учёта ограничения на количество отображаемых строк.
        std::wstring tempWholeChat = wholeChat + L'\n' +
                StringUtils::wrapByLineLength(
                        StringUtils::encodeFormatting(message),
                        MAX_LINE_LENGTH, 9999);

        // Затем отбрасываем наиболее старые сообщения.
        tempWholeChat += L'\n'; // добавляем в конец '\n', чтобы нормально выделились все строки (в т.ч. последняя)
        std::vector<std::wstring> lines;
        std::wstring currentLine;

        for (const wchar_t ch : tempWholeChat) {
            if (ch == L'\n') {
                if (!currentLine.empty()) { // может быть, если в сообщении несколько '\n' идут подряд
                    lines.push_back(currentLine);
                    currentLine = L"";
                }
            } else
                currentLine += ch;
        }

        wholeChat = L""; // сброс
        uint32_t linesNum = lines.size();
        uint32_t begin = linesNum <= MAX_LINES ? linesNum : linesNum - MAX_LINES;

        for (uint32_t i = begin; i < MAX_LINES; i++)
            wholeChat += lines[i] + L'\n';

        // Наконец, обновляем то, что видит игрок на экране
        RenderUtils::enrichText(*text, wholeChat);
    }

}
