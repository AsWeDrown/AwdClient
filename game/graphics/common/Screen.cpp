#include "Screen.hpp"


namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PROTECTED
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Screen::addComponent(const std::shared_ptr<Drawable>& component) {
        addChild(component);
        componentsIds.push_back(component->getId());
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Screen::Screen(id_type id,
                   float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window)
                   : Drawable(id, renderScale, window) {}

    void Screen::update() {
        Drawable::update();
    }

    void Screen::draw() {
        Drawable::draw();
    }

    void Screen::openDialog(const std::shared_ptr<Dialog>& dialog) {
        setComponentsEnabled(false);
        currentDialogId = dialog->getId();
        enqueueChild(dialog); // исп. "enqueue", т.к. нельзя исп. "add" во время обработки mouseClicked
    }

    void Screen::closeCurrentDialog() {
        if (isDialogOpen()) {
            auto currentDialog = std::dynamic_pointer_cast<Dialog>(getChildById(currentDialogId));
            currentDialog->hide();
        }
    }

    void Screen::dialogClosed() {
        removeChild(currentDialogId);
        currentDialogId = 0;
        setComponentsEnabled(true);
    }

    bool Screen::isDialogOpen() const {
        return currentDialogId != 0;
    }

    void Screen::setComponentsEnabled(bool enabled) {
        for (id_type componentId : componentsIds)
            getChildById(componentId)->setEnabled(enabled);
    }

}
