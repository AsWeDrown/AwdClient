#pragma once


#include "../common/Button.hpp"
#include "../common/Drawable.hpp"
#include "../common/Dialog.hpp"
#include "MainMenuScreenListener.hpp"

namespace awd::game {

    class MainMenuScreen : public Drawable {
    private:
        std::shared_ptr<MainMenuScreenListener> listener = std::make_shared<MainMenuScreenListener>();
        bool dialogOpen = false;
        std::vector<id_type> buttonsIds;

        void createButtons();
        void addButton(const std::shared_ptr<Drawable>& button);
        void setButtonsEnabled(bool enabled);

    public:
        MainMenuScreen(float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;

        bool isDialogOpen() const;
        void setDialogOpen(bool open);
        std::shared_ptr<MainMenuScreenListener> getListener() const;
    };

}

