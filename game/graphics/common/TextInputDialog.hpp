#pragma once


#include "Dialog.hpp"
#include "TextButton.hpp"
#include "NoPayload.hpp"

namespace awd::game {

    class TextInputDialog : public Dialog {
    private:
        id_type btnNextId;
        std::shared_ptr<ButtonListener<std::wstring>> btnNextListener = nullptr;

        id_type btnBackId;
        std::shared_ptr<ButtonListener<NoPayload>> btnBackListener = nullptr;

        void createBasicButtons();

    protected:
        void update() override;
        void draw() override;

    public:
        TextInputDialog(id_type id,
                        float renderScale,
                        const std::shared_ptr<sf::RenderWindow>& window,
                        const std::shared_ptr<DialogListener>& dialogListener,
                        id_type btnNextId,
                        const std::shared_ptr<ButtonListener<std::wstring>>& btnNextListener,
                        id_type btnBackId,
                        const std::shared_ptr<ButtonListener<NoPayload>>& btnBackListener);
    };

}

