// Фон окна (диалога)
#define INITIAL_Y_SHIFT_FACTOR 0.95f
#define DIALOG_WIDTH 800.0f
#define DIALOG_HEIGHT 350.0f
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
                   const std::shared_ptr<DialogListener>& listener)
                   : Drawable(id) {
        this->listener = listener;

        width        = DIALOG_WIDTH  * renderScale;
        height       = DIALOG_HEIGHT * renderScale;
        x            = window->getSize().x / 2.0f - width  / 2.0f; // NOLINT(cppcoreguidelines-narrowing-conversions)
        finalY       = window->getSize().y / 2.0f - height / 2.0f; // NOLINT(cppcoreguidelines-narrowing-conversions)
        y = initialY = finalY * INITIAL_Y_SHIFT_FACTOR;
        moveStep     = (finalY - initialY) / MAX_EFFECTIVE_EXISTED_TICKS;

        fill = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
        fill->setPosition(x, y);
    }

    void Dialog::onRegister() {
        // Диалоги создаются отложенно (через "enqueueAdd"),
        // так что используем onRegister для начала появления.
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

        // Для плавного появления окна сверху / исчезновения окна вверх.
        if (state == DialogState::APPEARING) {
            if (existedTicks < MAX_EFFECTIVE_EXISTED_TICKS) {
                y += moveStep;
                existedTicks++;
            } else {
                if (y != finalY)
                    // Из-за неточности вычислений с float окно может немного
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
                if (y != initialY)
                    // Из-за неточности вычислений с float окно может немного
                    // не доходить до нужной точки. Поэтому "помогаем" ему.
                    y = initialY;

                state = DialogState::DISAPPEARED;
                listener->dialogClosed(parent, id);
            }
        }

        uint32_t fillAlpha = FILL_ALPHA_PER_EXISTED_TICK * existedTicks;
        fill->setFillColor(sf::Color(0, 0, 0, fillAlpha));
        fill->setPosition(x, y);
    }

    void Dialog::draw() {
        // Сначала рисуем фон...
        window->draw(*fill);

        // ...а уже поверх него - всё остальное. Однако всё остальное (дочерние компоненты)
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
