#define DIALOG_WIDTH 1000
#define DIALOG_HEIGHT 400
#define MAX_EFFECTIVE_EXISTED_TICKS 5
#define FILL_ALPHA_PER_EXISTED_TICK 30


#include "Dialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Dialog::Dialog(float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window) {
        this->renderScale = renderScale;
        this->window = window;

        width  = DIALOG_WIDTH  * renderScale;
        height = DIALOG_HEIGHT * renderScale;
        x = window->getSize().x / 2 - width  / 2;
        y = window->getSize().y / 2 - height / 2;
    }

    void Dialog::addButton(const std::shared_ptr<Button>& button) {
        buttons.push_back(button);
        children.push_back(button);
    }

    void Dialog::update() {
        if (state == DialogState::DISAPPEARED)
            return;

        Drawable::update();

        if (state == DialogState::APPEARING) {
            if (existedTicks < MAX_EFFECTIVE_EXISTED_TICKS)
                existedTicks++;
            else
                state = DialogState::APPEARED;
        } else if (state == DialogState::DISAPPEARING) {
            if (existedTicks > 0)
                existedTicks--;
            else
                state = DialogState::DISAPPEARED;
        }
    }

    void Dialog::draw() {
        if (state == DialogState::DISAPPEARED)
            return;

        Drawable::draw();

        unsigned int fillAlpha = FILL_ALPHA_PER_EXISTED_TICK * existedTicks;

        sf::RectangleShape fill(sf::Vector2f(width, height));
        fill.setPosition(x, y);
        fill.setFillColor(sf::Color(0, 0, 0, fillAlpha));
        window->draw(fill);
    }

    void Dialog::show() {
        if (state == DialogState::DISAPPEARED)
            state = DialogState::APPEARING;
    }

    void Dialog::hide() {
        if (state == DialogState::APPEARED)
            state = DialogState::DISAPPEARING;
    }

    DialogState Dialog::getState() const {
        return state;
    }

}
