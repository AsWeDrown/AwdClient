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
        unsigned int initialY, finalY;
        std::shared_ptr<DialogListener> listener = nullptr;
        DialogState state = DialogState::DISAPPEARED;
        int existedTicks = 0;

    public:
        Dialog(id_type id,
               float renderScale,
               const std::shared_ptr<sf::RenderWindow>& window,
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
