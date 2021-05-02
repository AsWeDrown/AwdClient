#pragma once


#include "Dialog.hpp"
#include "TextButton.hpp"
#include "TextField.hpp"
#include "RichText.hpp"

namespace awd::game {

    class TextInputDialog : public Dialog {
    private:
        std::wstring message;

        id_type textFieldId;
        std::shared_ptr<TextFieldListener> textFieldListener = nullptr;
        unsigned int maxInputLen;
        std::wstring hintText;

        id_type btnNextId;
        std::shared_ptr<ButtonListener> btnNextListener = nullptr;

        id_type btnBackId;
        std::shared_ptr<ButtonListener> btnBackListener = nullptr;

        static void applyFormatting(sfe::RichText& msg, wchar_t code);

        void createTextField();
        void createBasicButtons();

    protected:
        void update() override;
        void draw() override;

    public:
        TextInputDialog(id_type id,
                        float renderScale,
                        const std::shared_ptr<sf::RenderWindow>& window,
                        const std::shared_ptr<DialogListener>& dialogListener,
                        const std::wstring& message,
                        id_type textFieldId,
                        const std::shared_ptr<TextFieldListener>& textFieldListener,
                        unsigned int maxInputLen,
                        const std::wstring& hintText,
                        id_type btnNextId,
                        const std::shared_ptr<ButtonListener>& btnNextListener,
                        id_type btnBackId,
                        const std::shared_ptr<ButtonListener>& btnBackListener);
    };

}

