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
        void (*backClicked)(Drawable* parentScreen);

        void update() override;
        void draw() override;

    public:
        TextInputDialog(int id,
                        float renderScale,
                        const std::shared_ptr<sf::RenderWindow>& window,
                        void (*dialogOpened)(Drawable*, int),
                        void (*dialogClosed)(Drawable*, int),
                        void (*nextClicked)(Drawable*, const std::wstring&),
                        void (*backClicked)(Drawable*));
    };

}

