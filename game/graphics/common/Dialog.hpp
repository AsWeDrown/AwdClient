#pragma once


#include "../common/Drawable.hpp"
#include "Button.hpp"

namespace awd::game {

    enum class DialogState {
        APPEARING,
        APPEARED,
        DISAPPEARING,
        DISAPPEARED
    };

    class Dialog : public Drawable {
    protected:
        void (*dialogOpened)(Drawable* parentScreen, int dialogId);
        void (*dialogClosed)(Drawable* parentScreen, int dialogId);
        DialogState state = DialogState::DISAPPEARED;
        int existedTicks = 0;

    public:
        Dialog(int id,
               float renderScale,
               const std::shared_ptr<sf::RenderWindow>& window,
               void (*dialogOpened)(Drawable*, int),
               void (*dialogClosed)(Drawable*, int));

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        void show();
        void hide();
        DialogState getState() const;
    };

}
