// Общее
#define WATER_FLOW_VELOCITY 6
// Фликеры
#define NUMBER_OF_FLICKERS 100
#define FLICKER_WIDTH 90
#define FLICKER_HEIGHT 3
#define MIN_FLICKER_MARGIN 30
#define MAX_FLICKER_MARGIN 60
// Пузырьки
#define NUMBER_OF_BUBBLES 10
#define MIN_BUBBLE_RADIUS 5
#define MAX_BUBBLE_RADIUS 10
#define BUBBLE_OUTLINE_THICKNESS 3
#define MIN_HORIZONTAL_BUBBLE_VELOCITY WATER_FLOW_VELOCITY
#define MAX_HORIZONTAL_BUBBLE_VELOCITY 12
#define MIN_VERTICAL_BUBBLE_VELOCITY 3
#define MAX_VERTICAL_BUBBLE_VELOCITY 3
#define MIN_BUBBLE_MARGIN 100
#define MAX_BUBBLE_MARGIN 200


#include "WaterBackground.hpp"
#include "ColorSet.hpp"
#include "RenderUtils.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void WaterBackground::createFlickers() {
        unsigned int fNum    = NUMBER_OF_FLICKERS * renderScale;
        unsigned int fWidth  = FLICKER_WIDTH      * renderScale;
        unsigned int fHeight = FLICKER_HEIGHT     * renderScale;
        unsigned int fMinMar = MIN_FLICKER_MARGIN * renderScale;
        unsigned int fMaxMar = MAX_FLICKER_MARGIN * renderScale;

        for (int i = 0; i < fNum; i++) {
            unsigned int fX       = Game::randUInt(0, width);
            unsigned int fOriginY = height / fNum * i;
            unsigned int fMargin  = Game::randUInt(fMinMar, fMaxMar) * renderScale;
            unsigned int fY       = fOriginY + fMargin;

            auto flicker = std::make_shared<sf::RectangleShape>(sf::Vector2f(fWidth, fHeight));
            flicker->setPosition(fX, fY);
            flicker->setFillColor(ColorSet::WATER_FLICKERS);
            flickers.push_back(flicker);
        }
    }

    void WaterBackground::createBubbles() {
        unsigned int bNum       = NUMBER_OF_BUBBLES        * renderScale;
        unsigned int bMinRadius = MIN_BUBBLE_RADIUS        * renderScale;
        unsigned int bMaxRadius = MAX_BUBBLE_RADIUS        * renderScale;
        unsigned int bMinMar    = MIN_BUBBLE_MARGIN        * renderScale;
        unsigned int bMaxMar    = MAX_BUBBLE_MARGIN        * renderScale;
        unsigned int bOutline   = BUBBLE_OUTLINE_THICKNESS * renderScale;

        for (int i = 0; i < bNum; i++) {
            unsigned int bRadius  = Game::randUInt(bMinRadius, bMaxRadius);
            unsigned int bX       = Game::randUInt(0, width);
            unsigned int bOriginY = height / bNum * i;
            unsigned int bMargin  = Game::randUInt(bMinMar, bMaxMar) * renderScale;
            unsigned int bY       = bOriginY + bMargin;

            auto bubble = std::make_shared<sf::CircleShape>(bRadius);
            bubble->setPosition(bX, bY);
            bubble->setFillColor(ColorSet::WATER_BUBBLES_FILL);
            bubble->setOutlineColor(ColorSet::WATER_BUBBLES_OUTLINE);
            bubble->setOutlineThickness(bOutline);
            bubbles.push_back(bubble);
        }
    }

    void WaterBackground::updateFlickers() {
        unsigned int fWidth       = FLICKER_WIDTH       * renderScale;
        unsigned int fHeight      = FLICKER_HEIGHT      * renderScale;
        unsigned int flowVelocity = WATER_FLOW_VELOCITY * renderScale;

        for (const auto& flicker : flickers) {
            if (flicker->getPosition().x < width) {
                auto thisSize = flicker->getSize();

                if (thisSize.x < fWidth)
                    // Просто увеличиваем размер. При увеличении размера фигуры дополняются
                    // справа, так что это создаст эффект постепенного появления фликера.
                    flicker->setSize(sf::Vector2f(thisSize.x + flowVelocity, fHeight));
                else
                    // Передвигаем фликер, уже достигший максимального размера, вправо.
                    flicker->move(flowVelocity, 0);
            } else {
                // Сбрасываем размер фликера и перемещаем его в левый край экрана.
                flicker->setSize(sf::Vector2f(1, fHeight));
                flicker->setPosition(0, flicker->getPosition().y);
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

    WaterBackground::WaterBackground(int id,
                                     float renderScale,
                                     const std::shared_ptr<sf::RenderWindow>& window) {
        this->id = id;
        this->renderScale = renderScale;
        this->window = window;

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
            window->draw(*flicker);

        // Пузырьки.
        for (const auto& bubble : bubbles)
            window->draw(*bubble);
    }

}
