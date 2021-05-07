#pragma once


#include "../common/Button.hpp"
#include "../common/Drawable.hpp"
#include "../common/Dialog.hpp"

namespace awd::game {

    class Screen : public Drawable {
    protected:
        id_type currentDialogId = 0;
        std::vector<id_type> componentsIds;

        void addComponent(const std::shared_ptr<Drawable>& component);

    public:
        Screen(id_type id,
               float renderScale,
               const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;

        void openDialog(const std::shared_ptr<Dialog>& dialog);
        void closeCurrentDialog();
        void dialogClosed();
        bool isDialogOpen() const;
        void setComponentsEnabled(bool enabled);
    };

}

