// Фон диалога
#define DIALOG_WIDTH 800
#define DIALOG_HEIGHT 300
#define MAX_EFFECTIVE_EXISTED_TICKS 3
#define FILL_ALPHA_PER_EXISTED_TICK 50


#include "Dialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Dialog::Dialog(int id,
                   float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window,
                   void (*dialogOpened)(Drawable*, int),
                   void (*dialogClosed)(Drawable*, int)) {
        this->id = id;
        this->renderScale = renderScale;
        this->window = window;
        this->dialogOpened = dialogOpened;
        this->dialogClosed = dialogClosed;

        width  = DIALOG_WIDTH  * renderScale;
        height = DIALOG_HEIGHT * renderScale;
        x = window->getSize().x / 2 - width  / 2;
        y = window->getSize().y / 2 - height / 2;
    }

    void Dialog::keyPressed(const sf::Event::KeyEvent& event) {
        if (state == DialogState::APPEARED)
            Drawable::keyPressed(event);
    }

    void Dialog::mousePressed(const sf::Event::MouseButtonEvent& event) {
        if (state == DialogState::APPEARED)
            Drawable::mousePressed(event);
    }

    void Dialog::update() {
        if (state == DialogState::DISAPPEARED)
            return;

        Drawable::update();

        if (state == DialogState::APPEARING) {
            if (existedTicks < MAX_EFFECTIVE_EXISTED_TICKS)
                existedTicks++;
            else {
                state = DialogState::APPEARED;
                dialogOpened(parent, id);
            }
        } else if (state == DialogState::DISAPPEARING) {
            if (existedTicks > 0)
                existedTicks--;
            else {
                state = DialogState::DISAPPEARED;
                dialogClosed(parent, id);
            }
        }
    }

    void Dialog::draw() {
        // Сначала рисуем фон...
        unsigned int fillAlpha = FILL_ALPHA_PER_EXISTED_TICK * existedTicks;

        sf::RectangleShape fill(sf::Vector2f(width, height));
        fill.setPosition(x, y);
        fill.setFillColor(sf::Color(0, 0, 0, fillAlpha));
        window->draw(fill);

        // ...и уже поверх него - всё остальное. Однако всё остальное (дочерние компоненты)
        // рисуем только в случае, если это окно в процессе отображения или уже полностью
        // отображено (APPEARING или DISAPPEARING соответственно). При этом на стадии
        // DISAPPEARING мы "всё остальное" уже не рисуем. Так достигается наилучшее
        // впечатление для пользователя (взаимодействие с окном так наиболее приятно глазу).
        if (state == DialogState::APPEARING || state == DialogState::APPEARED)
            Drawable::draw();
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
