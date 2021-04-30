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
        std::vector<std::shared_ptr<Button>> buttons;
        DialogState state = DialogState::APPEARING;
        int existedTicks = 0;

        virtual void createButtons() = 0;
        void addButton(const std::shared_ptr<Button>& button);

    public:
        Dialog(float renderScale,
               const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;

        void show();
        void hide();
        DialogState getState() const;
    };

}
