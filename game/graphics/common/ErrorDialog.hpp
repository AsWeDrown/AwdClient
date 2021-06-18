#pragma once


#include "Dialog.hpp"
#include "TextField.hpp"
#include "RichText.hpp"

namespace awd::game {

    class ErrorDialog : public Dialog {
    private:
        id_type btnOkId;

        std::unique_ptr<sf::RectangleShape> sepLine = nullptr;
        std::unique_ptr<sfe::RichText>      text    = nullptr;

    public:
        ErrorDialog(id_type id,
                    const std::shared_ptr<DialogListener>& dialogListener,
                    const std::wstring& message,
                    id_type btnOkId,
                    const std::shared_ptr<ButtonListener>& btnOkListener);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void update() override;
        void draw() override;
    };

}

