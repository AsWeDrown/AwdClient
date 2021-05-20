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

    Screen::Screen(id_type id)
                   : Drawable(id) {
        this->x      = 0.0f;
        this->y      = 0.0f;
        this->width  = window->getSize().x; // NOLINT(cppcoreguidelines-narrowing-conversions)
        this->height = window->getSize().y; // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    void Screen::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);
    }

    void Screen::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);
    }

    void Screen::update() {
        Drawable::update();
    }

    void Screen::draw() {
        Drawable::draw();
    }

    void Screen::showLoadingOverlay(const std::shared_ptr<LoadingOverlay>& loadingOverlay) {
        hideCurrentLoadingOverlay();
        setComponentsEnabled(false);
        currentLoadingOverlayId = loadingOverlay->getId();
        enqueueAddChild(loadingOverlay); // т.к. нельзя исп. "add" во время обработки событий
    }

    void Screen::hideCurrentLoadingOverlay() {
        if (isLoadingOverlayShown()) {
            enqueueRemoveChild(currentLoadingOverlayId); // т.к. нельзя "remove" во время обработки событий
            currentLoadingOverlayId = 0;
            setComponentsEnabled(true);
        }
    }

    bool Screen::isLoadingOverlayShown() const {
        return currentLoadingOverlayId != 0;
    }

    void Screen::openDialog(const std::shared_ptr<Dialog>& dialog) {
        closeCurrentDialog();
        setComponentsEnabled(false);
        currentDialogId = dialog->getId();
        enqueueAddChild(dialog); // исп. "enqueueAdd", т.к. нельзя исп. "add" во время обработки событий
    }

    void Screen::closeCurrentDialog() {
        if (isDialogOpen()) {
            auto currentDialog = std::dynamic_pointer_cast
                    <Dialog>(getChildById(currentDialogId));
            currentDialog->hide();
        }
    }

    void Screen::dialogClosed() {
        enqueueRemoveChild(currentDialogId); // "enqueueRemove", т.к. нельзя "remove" во время обработки событий
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
