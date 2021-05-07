// Фон окна (диалога)
#define INITIAL_Y_SHIFT_FACTOR 0.95f
#define DIALOG_WIDTH 800
#define DIALOG_HEIGHT 350
#define MAX_EFFECTIVE_EXISTED_TICKS 4
#define FILL_ALPHA_PER_EXISTED_TICK 35


#include "Dialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Dialog::Dialog(id_type id,
                   float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window,
                   const std::shared_ptr<DialogListener>& listener)
                   : Drawable(id, renderScale, window) {
        this->listener = listener;

        width        = DIALOG_WIDTH  * renderScale;
        height       = DIALOG_HEIGHT * renderScale;
        x            = window->getSize().x / 2 - width  / 2;
        finalY       = window->getSize().y / 2 - height / 2;
        y = initialY = finalY * INITIAL_Y_SHIFT_FACTOR;
    }

    void Dialog::onRegister() {
        // Диалоги обычно создаются отложенно (через "enqueue"), так что используем onRegister для старта.
        show();
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

        // Для плавного появления окна сверху / исчезновения окна вверх (см. ниже).
        unsigned int moveStep = (finalY - initialY) / MAX_EFFECTIVE_EXISTED_TICKS;

        if (state == DialogState::APPEARING) {
            if (existedTicks < MAX_EFFECTIVE_EXISTED_TICKS) {
                y += moveStep;
                existedTicks++;
            } else {
                if (y < finalY)
                    // Из-за неделимости целых чисел окно может чуть-чуть
                    // не доходить до нужной точки. Поэтому "помогаем" ему.
                    y = finalY;

                state = DialogState::APPEARED;
                listener->dialogOpened(parent, id);
            }
        } else if (state == DialogState::DISAPPEARING) {
            if (existedTicks > 0) {
                y -= moveStep;
                existedTicks--;
            } else {
                if (y > initialY)
                    // Из-за неделимости целых чисел окно может чуть-чуть
                    // не доходить до нужной точки. Поэтому "помогаем" ему.
                    y = initialY;

                state = DialogState::DISAPPEARED;
                listener->dialogClosed(parent, id);
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

}
