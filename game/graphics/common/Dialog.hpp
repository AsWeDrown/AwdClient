#pragma once


#include "Button.hpp"
#include "DialogListener.hpp"

namespace awd::game {

    enum class DialogState {
        APPEARING,
        APPEARED,
        DISAPPEARING,
        DISAPPEARED
    };

    class Dialog : public Drawable {
    protected:
        float initialY, finalY, moveStep;
        std::shared_ptr<DialogListener> listener = nullptr;
        DialogState state = DialogState::DISAPPEARED;
        uint32_t existedTicks = 0;

        std::unique_ptr<sf::RectangleShape> fill = nullptr;

    public:
        Dialog(id_type id,
               const std::shared_ptr<DialogListener>& listener);

        void onRegister() override;
        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        void show();
        void hide();
    };

}
