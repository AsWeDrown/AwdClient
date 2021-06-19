// Фон
#define RECT_ALPHA 150
#define RECT_WIDTH 390.0f
// Текст
#define TEXT_FONT_SIZE 27
#define TEXT_LEFT_MARGIN 10.0f
#define MAX_LINE_LENGTH 28
#define MAX_LINES 4


#include "QuestBalloon.hpp"
#include "../../Game.hpp"
#include "../../util/RenderUtils.hpp"
#include "../../util/StringUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void QuestBalloon::relocate() {
        sf::Vector2f viewCenter = window->getView().getCenter();
        sf::Vector2f viewSize = window->getView().getSize();
        float viewX = viewCenter.x - viewSize.x / 2.0f;
        float viewY = viewCenter.y - viewSize.y / 2.0f;

        // Фон
        rect->setPosition(viewX + viewSize.x - width, viewY + totalTopMargin);

        // Текст
        float textLeftMargin = TEXT_LEFT_MARGIN * renderScale;
        sf::Rect<float> textBounds = text->getGlobalBounds();
        float textX = rect->getPosition().x + textLeftMargin;
        float textY = viewY + totalTopMargin + height / 2.0f - textBounds.height / 2.0f;

        text->setPosition(textX, textY);
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    QuestBalloon::QuestBalloon(id_type id,
                               float height,
                               float totalTopMargin,
                               const std::shared_ptr<Quest>& quest)
                               : Drawable(id) {
        this->height = height;
        this->totalTopMargin = totalTopMargin;
        this->quest = quest;

        // Фон
        float rectWidth  = RECT_WIDTH * renderScale;

        width = rectWidth;
        rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
        rect->setFillColor(sf::Color(0, 0, 0, RECT_ALPHA));

        // Текст
        uint32_t textFontSize = TEXT_FONT_SIZE * renderScale;

        text = std::make_unique<sfe::RichText>(*Game::instance().getFonts()->regularFont);
        text->setCharacterSize(textFontSize);
        updateProgress(0);

        // Стартовая позиция
        relocate();
    }

    void QuestBalloon::update() {
        Drawable::update();
        relocate();
    }

    void QuestBalloon::draw() {
        Drawable::draw();

        window->draw(*rect);
        window->draw(*text);
    }

    uint32_t QuestBalloon::getProgress() const {
        return progress;
    }

    void QuestBalloon::updateProgress(uint32_t newProgress) {
        if (this->progress != newProgress) {
            this->progress = newProgress;

            std::wstring msg = StringUtils::wrapByLineLength(StringUtils::encodeFormatting(
                    L"{GRAY}" + quest->getDesc() + L"{WHITE} | Выполнено: {YELLOW}"
                    + std::to_wstring(progress) + L" из " + std::to_wstring(quest->getMaxProgress())
            ), MAX_LINE_LENGTH, MAX_LINES);

            text->clear();
            RenderUtils::enrichText(*text, msg);
        }
    }

}
