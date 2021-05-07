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
        Screen(id_type id,
               float renderScale,
               const std::shared_ptr<sf::RenderWindow>& window);

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

