#pragma once


#include "Drawable.hpp"
#include "Dialog.hpp"

namespace awd::game {

    class Screen : public Drawable {
    protected:
        std::shared_ptr<Dialog> currentDialog = nullptr;

    public:
        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        std::shared_ptr<Dialog> getCurrentDialog() const;
        void setCurrentDialog(const std::shared_ptr<Dialog>& newDialog);
        void closeCurrentDialog();
    };

}

