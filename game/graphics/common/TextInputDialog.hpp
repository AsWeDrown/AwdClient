#pragma once


#include "Dialog.hpp"
#include "TextButton.hpp"
#include "NoPayload.hpp"
#include "TextField.hpp"

namespace awd::game {

    class TextInputDialogListener : public TextFieldListener {
    private:
        Drawable* textInputDialogParent;
        id_type btnNextId;

    public:
        TextInputDialogListener(Drawable* textInputDialogParent,
                                id_type btnNextId);

        void contentsChanged(Drawable* textFieldParent, id_type textFieldId,
                             const std::wstring& newContents) override;
    };

    class TextInputDialog : public Dialog {
    private:
        id_type textFieldId;
        unsigned int maxInputLen;
        std::wstring hintText;
        std::shared_ptr<TextInputDialogListener> textFieldListener = nullptr;

        id_type btnNextId;
        std::shared_ptr<ButtonListener<std::wstring>> btnNextListener = nullptr;

        id_type btnBackId;
        std::shared_ptr<ButtonListener<NoPayload>> btnBackListener = nullptr;

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
                        Drawable* textInputDialogParent,
                        id_type textFieldId,
                        unsigned int maxInputLen,
                        const std::wstring& hintText,
                        id_type btnNextId,
                        const std::shared_ptr<ButtonListener<std::wstring>>& btnNextListener,
                        id_type btnBackId,
                        const std::shared_ptr<ButtonListener<NoPayload>>& btnBackListener);
    };

}

