#pragma once


#include "Dialog.hpp"
#include "TextField.hpp"
#include "RichText.hpp"

namespace awd::game {

    class TextInputDialog : public Dialog {
    private:
        float buttonsY        = 0.0f,
              textFieldY      = 0.0f,
              textFieldHeight = 0.0f;

        id_type textFieldId;
        std::shared_ptr<TextFieldListener> textFieldListener = nullptr;
        uint32_t maxInputLen;
        std::wstring hintText, initialInput;

        id_type btnNextId;
        std::shared_ptr<ButtonListener> btnNextListener = nullptr;

        id_type btnBackId;
        std::shared_ptr<ButtonListener> btnBackListener = nullptr;

        std::unique_ptr<sf::RectangleShape> sep = nullptr;
        std::unique_ptr<sfe::RichText>      msg = nullptr;

        void createTextField();
        void createBasicButtons();

    public:
        TextInputDialog(id_type id,
                        float renderScale,
                        const std::shared_ptr<sf::RenderWindow>& window,
                        const std::shared_ptr<DialogListener>& dialogListener,
                        const std::wstring& message,
                        id_type textFieldId,
                        const std::shared_ptr<TextFieldListener>& textFieldListener,
                        uint32_t maxInputLen,
                        const std::wstring& hintText,
                        const std::wstring& initialInput,
                        id_type btnNextId,
                        const std::shared_ptr<ButtonListener>& btnNextListener,
                        id_type btnBackId,
                        const std::shared_ptr<ButtonListener>& btnBackListener);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void update() override;
        void draw() override;
    };

}

