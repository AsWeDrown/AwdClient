#pragma once


#include "../common/Button.hpp"
#include "../common/Drawable.hpp"
#include "../common/Dialog.hpp"
#include "LoadingOverlay.hpp"

namespace awd::game {

    class Screen : public Drawable {
    protected:
        id_type currentLoadingOverlayId = 0;
        id_type currentDialogId = 0;
        std::vector<id_type> componentsIds;

        void addComponent(const std::shared_ptr<Drawable>& component);

    public:
        explicit Screen(id_type id);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        void showLoadingOverlay(const std::shared_ptr<LoadingOverlay>& loadingOverlay);
        void hideCurrentLoadingOverlay();
        bool isLoadingOverlayShown() const;

        void openDialog(const std::shared_ptr<Dialog>& dialog);
        void closeCurrentDialog();
        void dialogClosed();
        bool isDialogOpen() const;

        void setComponentsEnabled(bool enabled);
    };

}

