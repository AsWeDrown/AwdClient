#include "Screen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Screen::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);

        if (currentDialog != nullptr)
            currentDialog->keyPressed(event);
    }

    void Screen::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);

        if (currentDialog != nullptr)
            currentDialog->mousePressed(event);
    }

    void Screen::update() {
        Drawable::update();

        if (currentDialog != nullptr)
            currentDialog->update();
    }

    void Screen::draw() {
        Drawable::draw();

        if (currentDialog != nullptr)
            currentDialog->draw();
    }

    std::shared_ptr<Dialog> Screen::getCurrentDialog() const {
        return currentDialog;
    }

    void Screen::setCurrentDialog(const std::shared_ptr<Dialog>& newDialog) {
        this->currentDialog = newDialog;
    }

    void Screen::closeCurrentDialog() {
        currentDialog->hide();
        currentDialog = nullptr;
    }

}
