#pragma once


#include "../common/Dialog.hpp"

namespace awd::game {

    class EscPlayDialog : public Dialog {
    private:
        std::unique_ptr<sf::RectangleShape> rect = nullptr; // Затемнение фона

    public:
        EscPlayDialog(const std::shared_ptr<DialogListener>& dialogListener,
                      const std::shared_ptr<ButtonListener>& btnResumeListener,
                      const std::shared_ptr<ButtonListener>& btnQuitListener);

        void update() override;
        void draw() override;
    };

}

