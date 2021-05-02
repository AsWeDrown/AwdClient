// Общее
#define WATER_FLOW_VELOCITY 6
#define MIN_DISTANCE_FACTOR 0.45f
#define MAX_DISTANCE_FACTOR 1.05f
// Фликеры
#define FLICKER_FILL_ALPHA 65
#define FLICKER_WIDTH 80
#define FLICKER_HEIGHT 4
#define FLICKERS_HORIZONTAL_MARGIN 110 /* нужно для генерации ячеек для фликеров (сетка) */
#define FLICKERS_VERTICAL_MARGIN 135 /* нужно для генерации ячеек для фликеров (сетка) */
#define FLICKERS_CELL_WIDTH FLICKER_WIDTH * MAX_DISTANCE_FACTOR + FLICKERS_HORIZONTAL_MARGIN
#define FLICKERS_CELL_HEIGHT FLICKER_HEIGHT * MAX_DISTANCE_FACTOR + FLICKERS_VERTICAL_MARGIN
#define FLICKERS_PLACEMENT_MARGIN_DIV 10 /* нужно для нормальной рандомизации фликеров внутри сгенерированных ячеек */
// Пузырьки
#define BUBBLE_FILL_ALPHA 45
#define BUBBLE_OUTLINE_ALPHA 80
#define BUBBLE_OUTLINE_THICKNESS 2
#define NUMBER_OF_BUBBLES 10
#define BUBBLE_RADIUS 10
#define MIN_HORIZONTAL_BUBBLE_VELOCITY WATER_FLOW_VELOCITY + 1
#define MAX_HORIZONTAL_BUBBLE_VELOCITY WATER_FLOW_VELOCITY + 3
#define MIN_VERTICAL_BUBBLE_VELOCITY 1
#define MAX_VERTICAL_BUBBLE_VELOCITY 3
#define MIN_BUBBLE_MARGIN 100
#define MAX_BUBBLE_MARGIN 200


#include "WaterBackground.hpp"
#include "ColorSet.hpp"
#include "RenderUtils.hpp"
#include "../../Game.hpp"
#include "UniformGridBuilder.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void WaterBackground::createFlickers() {
        unsigned int fWidth      = FLICKER_WIDTH              * renderScale;
        unsigned int fHeight     = FLICKER_HEIGHT             * renderScale;
        unsigned int fHorMargin  = FLICKERS_HORIZONTAL_MARGIN * renderScale;
        unsigned int fVerMargin  = FLICKERS_VERTICAL_MARGIN   * renderScale;
        unsigned int fCellWidth  = FLICKERS_CELL_WIDTH        * renderScale;
        unsigned int fCellHeight = FLICKERS_CELL_HEIGHT       * renderScale;

        sf::Vector2f fCellStart;
        UniformGridBuilder gridBuilder(0, 0, width, height,
                                       fCellWidth, fCellHeight, true);

        do {
            gridBuilder >> fCellStart;
            float distFactor = Game::randFloat(MIN_DISTANCE_FACTOR, MAX_DISTANCE_FACTOR);

            unsigned int thisFlickerWidth  = fWidth             * distFactor * distFactor;
            unsigned int thisFlickerHeight = fHeight            * distFactor + 1; // min 1 px
            unsigned int thisFlickerAlpha  = FLICKER_FILL_ALPHA * distFactor;

            unsigned int thisFlickerX = Game::randUInt(
                    fCellStart.x + fHorMargin / FLICKERS_PLACEMENT_MARGIN_DIV,
                    fCellStart.x + fCellWidth - thisFlickerWidth - fHorMargin / FLICKERS_PLACEMENT_MARGIN_DIV
            );

            unsigned int thisFlickerY = Game::randUInt(
                    fCellStart.y + fVerMargin / FLICKERS_PLACEMENT_MARGIN_DIV,
                    fCellStart.y + fCellHeight - thisFlickerHeight - fVerMargin / FLICKERS_PLACEMENT_MARGIN_DIV
            );

            sf::Color thisFlickerColor = ColorSet::WATER_FLICKERS;
            thisFlickerColor.a = thisFlickerAlpha;

            auto flicker = std::make_shared<sf::RectangleShape>(
                    sf::Vector2f(thisFlickerWidth, thisFlickerHeight));
            flicker->setPosition(thisFlickerX, thisFlickerY);
            flicker->setFillColor(thisFlickerColor);
            flickers.push_back(std::make_shared<RectWrapper>(
                    thisFlickerWidth, thisFlickerHeight, flicker));
        } while (!gridBuilder.failed()); // failed --> весь экран заполнен
    }

    void WaterBackground::createBubbles() {
        unsigned int bNum       = NUMBER_OF_BUBBLES        * renderScale;
        unsigned int bRadius    = BUBBLE_RADIUS            * renderScale;
        unsigned int bMinMar    = MIN_BUBBLE_MARGIN        * renderScale;
        unsigned int bMaxMar    = MAX_BUBBLE_MARGIN        * renderScale;
        unsigned int bOutline   = BUBBLE_OUTLINE_THICKNESS * renderScale + 1; // min 1 px

        for (int i = 0; i < bNum; i++) {
            float distFactor = Game::randFloat(MIN_DISTANCE_FACTOR, MAX_DISTANCE_FACTOR);

            unsigned int thisBubbleRadius       = bRadius * distFactor * distFactor;
            unsigned int thisBubbleX            = Game::randUInt(0, width);
            unsigned int bOriginY               = height / bNum * i;
            unsigned int bMargin                = Game::randUInt(bMinMar, bMaxMar) * renderScale;
            unsigned int thisBubbleY            = bOriginY + bMargin;
            unsigned int thisBubbleFillAlpha    = BUBBLE_FILL_ALPHA        * distFactor;
            unsigned int thisBubbleOutlineAlpha = BUBBLE_OUTLINE_ALPHA     * distFactor;

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
        unsigned int flowVelocity = WATER_FLOW_VELOCITY * renderScale;

        for (const auto& flicker : flickers) {
            unsigned int preservedWidth  = flicker->getPreservedWidth();
            unsigned int preservedHeight = flicker->getPreservedHeight();
            auto fRect = flicker->getRect();

            if (fRect->getPosition().x < width) {
                auto thisSize = fRect->getSize();

                if (thisSize.x < preservedWidth)
                    // Просто увеличиваем размер. При увеличении размера фигуры дополняются
                    // справа, так что это создаст эффект постепенного появления фликера.
                    fRect->setSize(sf::Vector2f(thisSize.x + flowVelocity, preservedHeight));
                else
                    // Передвигаем фликер, уже достигший максимального размера, вправо.
                    fRect->move(flowVelocity, 0);
            } else {
                // Сбрасываем размер фликера и перемещаем его в левый край экрана.
                fRect->setSize(sf::Vector2f(1, preservedHeight));
                fRect->setPosition(0, fRect->getPosition().y);
            }
        }
    }

    void WaterBackground::updateBubbles() {
        unsigned int bMinHorVelocity = MIN_HORIZONTAL_BUBBLE_VELOCITY * renderScale;
        unsigned int bMaxHorVelocity = MAX_HORIZONTAL_BUBBLE_VELOCITY * renderScale;
        unsigned int bMinVerVelocity = MIN_VERTICAL_BUBBLE_VELOCITY   * renderScale;
        unsigned int bMaxVerVelocity = MAX_VERTICAL_BUBBLE_VELOCITY   * renderScale;

        for (const auto& bubble : bubbles) {
            unsigned int bHorVelocity = Game::randUInt(bMinHorVelocity, bMaxHorVelocity);
            unsigned int bVerVelocity = Game::randUInt(bMinVerVelocity, bMaxVerVelocity);

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

    WaterBackground::WaterBackground(id_type id,
                                     float renderScale,
                                     const std::shared_ptr<sf::RenderWindow>& window)
                                     : Drawable(id, renderScale, window) {
        x = 0;
        y = 0;
        width = window->getSize().x;
        height = window->getSize().y;

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
            window->draw(*flicker->getRect());

        // Пузырьки.
        for (const auto& bubble : bubbles)
            window->draw(*bubble);
    }

}
