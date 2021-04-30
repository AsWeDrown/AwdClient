#pragma once


#include "Dialog.hpp"
#include "TextButton.hpp"

namespace awd::game {

    class TextInputDialog : public Dialog {
    private:
        static void localNextClicked(Drawable* buttonOwner);
        static void localBackClicked(Drawable* buttonOwner);

        void createBasicButtons();

    protected:
        void (*nextClicked)(Drawable* parentScreen, const std::wstring& userInput);

        void createButtons() override;
        void update() override;
        void draw() override;

    public:
        TextInputDialog(float renderScale,
                        const std::shared_ptr<sf::RenderWindow>& window,
                        void (*nextClicked)(Drawable* buttonOwner, const std::wstring& userInput));
    };

}

