// Общее
#define WATER_FLOW_VELOCITY 6.0f
#define MIN_DISTANCE_FACTOR 0.45f
#define MAX_DISTANCE_FACTOR 1.05f
// Фликеры
#define FLICKER_FILL_ALPHA 65
#define FLICKER_WIDTH 80.0f
#define FLICKER_HEIGHT 4.0f
#define FLICKERS_HORIZONTAL_MARGIN 110.0f /* нужно для генерации ячеек для фликеров (сетка) */
#define FLICKERS_VERTICAL_MARGIN 135.0f /* нужно для генерации ячеек для фликеров (сетка) */
#define FLICKERS_CELL_WIDTH FLICKER_WIDTH * MAX_DISTANCE_FACTOR + FLICKERS_HORIZONTAL_MARGIN
#define FLICKERS_CELL_HEIGHT FLICKER_HEIGHT * MAX_DISTANCE_FACTOR + FLICKERS_VERTICAL_MARGIN
#define FLICKERS_PLACEMENT_MARGIN_DIV 10.0f /* нужно для нормальной рандомизации фликеров внутри сгенерированных ячеек */
// Пузырьки
#define BUBBLE_FILL_ALPHA 45
#define BUBBLE_OUTLINE_ALPHA 80
#define BUBBLE_OUTLINE_THICKNESS 2.0f
#define NUMBER_OF_BUBBLES 10
#define BUBBLE_RADIUS 10.0f
#define MIN_HORIZONTAL_BUBBLE_VELOCITY WATER_FLOW_VELOCITY + 1.0f
#define MAX_HORIZONTAL_BUBBLE_VELOCITY WATER_FLOW_VELOCITY + 3.0f
#define MIN_VERTICAL_BUBBLE_VELOCITY 1.0f
#define MAX_VERTICAL_BUBBLE_VELOCITY 3.0f
#define MIN_BUBBLE_MARGIN 100.0f
#define MAX_BUBBLE_MARGIN 200.0f


#include "WaterBackground.hpp"
#include "ColorSet.hpp"
#include "../../util/RenderUtils.hpp"
#include "../../Game.hpp"
#include "UniformGridBuilder.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void WaterBackground::createFlickers() {
        float fWidth      = FLICKER_WIDTH              * renderScale;
        float fHeight     = FLICKER_HEIGHT             * renderScale;
        float fHorMargin  = FLICKERS_HORIZONTAL_MARGIN * renderScale;
        float fVerMargin  = FLICKERS_VERTICAL_MARGIN   * renderScale;
        float fCellWidth  = FLICKERS_CELL_WIDTH        * renderScale;
        float fCellHeight = FLICKERS_CELL_HEIGHT       * renderScale;

        sf::Vector2f fCellStart;
        UniformGridBuilder gridBuilder(0.0f, 0.0f, width, height,
                                       fCellWidth, fCellHeight, true);

        do {
            gridBuilder >> fCellStart;

            float distFactor = Game::randFloat(MIN_DISTANCE_FACTOR, MAX_DISTANCE_FACTOR);

            float    thisFlickerWidth        = fWidth             * distFactor * distFactor;
            float    thisFlickerHeight       = fHeight            * distFactor + 1.0f; // min 1 px
            uint32_t thisFlickerAlpha        = FLICKER_FILL_ALPHA * distFactor;

            float thisFlickerX = Game::randFloat(
                    fCellStart.x + fHorMargin / FLICKERS_PLACEMENT_MARGIN_DIV,
                    fCellStart.x + fCellWidth - thisFlickerWidth - fHorMargin / FLICKERS_PLACEMENT_MARGIN_DIV
            );

            float thisFlickerY = Game::randFloat(
                    fCellStart.y + fVerMargin / FLICKERS_PLACEMENT_MARGIN_DIV,
                    fCellStart.y + fCellHeight - thisFlickerHeight - fVerMargin / FLICKERS_PLACEMENT_MARGIN_DIV
            );

            sf::Color thisFlickerColor = ColorSet::WATER_FLICKERS;
            thisFlickerColor.a = thisFlickerAlpha;

            auto flicker = std::make_shared<sf::RectangleShape>(
                    sf::Vector2f(thisFlickerWidth, thisFlickerHeight));
            flicker->setPosition(thisFlickerX, thisFlickerY);
            flicker->setFillColor(thisFlickerColor);

            auto wrapper = std::make_shared<RectWrapper>();
            wrapper->preservedWidth  = thisFlickerWidth;
            wrapper->preservedHeight = thisFlickerHeight;
            wrapper->rect            = flicker;
            flickers.push_back(wrapper);
        } while (!gridBuilder.failed()); // failed --> весь экран заполнен
    }

    void WaterBackground::createBubbles() {
        uint32_t  bNum     = NUMBER_OF_BUBBLES        * renderScale;
        float     bRadius  = BUBBLE_RADIUS            * renderScale;
        float     bMinMar  = MIN_BUBBLE_MARGIN        * renderScale;
        float     bMaxMar  = MAX_BUBBLE_MARGIN        * renderScale;
        float     bOutline = BUBBLE_OUTLINE_THICKNESS * renderScale + 1.0f; // min 1 px

        for (int i = 0; i < bNum; i++) {
            float distFactor = Game::randFloat(MIN_DISTANCE_FACTOR, MAX_DISTANCE_FACTOR);

            float    thisBubbleRadius              = bRadius * distFactor * distFactor;
            float    thisBubbleX                   = Game::randFloat(0.0f, width);
            float    bOriginY                      = height / bNum * i; // NOLINT(cppcoreguidelines-narrowing-conversions)
            float    bMargin                       = Game::randFloat(bMinMar, bMaxMar) * renderScale;
            float    thisBubbleY                   = bOriginY + bMargin;
            uint32_t thisBubbleFillAlpha           = BUBBLE_FILL_ALPHA        * distFactor;
            uint32_t thisBubbleOutlineAlpha        = BUBBLE_OUTLINE_ALPHA     * distFactor;

            sf::Color thisBubbleFill = ColorSet::WATER_BUBBLES;
            thisBubbleFill.a = thisBubbleFillAlpha;
            sf::Color thisBubbleOutline = ColorSet::WATER_BUBBLES;
            thisBubbleOutline.a = thisBubbleOutlineAlpha;

            auto bubble = std::make_shared<sf::CircleShape>(thisBubbleRadius);
            bubble->setPosition(thisBubbleX, thisBubbleY);
            bubble->setFillColor(thisBubbleFill);
            bubble->setOutlineColor(thisBubbleOutline);
            bubble->setOutlineThickness(bOutline);
            bubbles.push_back(bubble);
        }
    }

    void WaterBackground::updateFlickers() {
        float flowVelocity = WATER_FLOW_VELOCITY * renderScale;

        for (const auto& flicker : flickers) {
            float preservedWidth  = flicker->preservedWidth;
            float preservedHeight = flicker->preservedHeight;
            auto fRect            = flicker->rect;

            if (fRect->getPosition().x < width) {
                auto thisSize = fRect->getSize();

                if (thisSize.x < preservedWidth)
                    // Просто увеличиваем размер. При увеличении размера фигуры дополняются
                    // справа, так что это создаст эффект постепенного появления фликера.
                    fRect->setSize(sf::Vector2f(thisSize.x + flowVelocity, preservedHeight));
                else
                    // Передвигаем фликер, уже достигший максимального размера, вправо.
                    fRect->move(flowVelocity, 0.0f);
            } else {
                // Сбрасываем размер фликера и перемещаем его в левый край экрана.
                fRect->setSize(sf::Vector2f(1.0f, preservedHeight));
                fRect->setPosition(0.0f, fRect->getPosition().y);
            }
        }
    }

    void WaterBackground::updateBubbles() {
        float bMinHorVelocity = MIN_HORIZONTAL_BUBBLE_VELOCITY * renderScale;
        float bMaxHorVelocity = MAX_HORIZONTAL_BUBBLE_VELOCITY * renderScale;
        float bMinVerVelocity = MIN_VERTICAL_BUBBLE_VELOCITY   * renderScale;
        float bMaxVerVelocity = MAX_VERTICAL_BUBBLE_VELOCITY   * renderScale;

        for (const auto& bubble : bubbles) {
            float bHorVelocity = Game::randFloat(bMinHorVelocity, bMaxHorVelocity);
            float bVerVelocity = Game::randFloat(bMinVerVelocity, bMaxVerVelocity);

            auto pos = bubble->getPosition();

            float newX = pos.x < width ? pos.x + bHorVelocity : 0;
            float newY = pos.y > 0     ? pos.y - bVerVelocity : height;

            bubble->setPosition(newX, newY);
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    WaterBackground::WaterBackground(id_type id)
                                     : Drawable(id) {
        x      = 0.0f;
        y      = 0.0f;
        width  = window->getSize().x; // NOLINT(cppcoreguidelines-narrowing-conversions)
        height = window->getSize().y; // NOLINT(cppcoreguidelines-narrowing-conversions)

        // Рандомно генерируем перемещающиеся объекты
        createFlickers();
        createBubbles();
    }

    void WaterBackground::update() {
        Drawable::update();

        updateFlickers();
        updateBubbles();
    }

    void WaterBackground::draw() {
        Drawable::draw();

        // Градиент воды.
        RenderUtils::verticalGradientSquare(window, 0, 0, width, height,
                                            ColorSet::HIGHEST_LEVEL_WATER, ColorSet::MIDDLE_LEVEL_WATER);

        // Иллюзия течения (фликеры).
        for (const auto& flicker : flickers)
            window->draw(*flicker->rect);

        // Пузырьки.
        for (const auto& bubble : bubbles)
            window->draw(*bubble);
    }

}
