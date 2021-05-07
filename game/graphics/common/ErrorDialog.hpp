#pragma once


#include "Dialog.hpp"
#include "TextButton.hpp"
#include "TextField.hpp"
#include "RichText.hpp"

namespace awd::game {

    class ErrorDialog : public Dialog {
    private:
        std::wstring message;
        id_type btnOkId;

    protected:
        void update() override;
        void draw() override;

    public:
        ErrorDialog(id_type id,
                    float renderScale,
                    const std::shared_ptr<sf::RenderWindow>& window,
                    const std::shared_ptr<DialogListener>& dialogListener,
                    const std::wstring& message,
                    id_type btnOkId,
                    const std::shared_ptr<ButtonListener>& btnOkListener);
    };

}
