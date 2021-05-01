#include <iostream>
#include "Button.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    template<typename TPayloadType>
    Button<TPayloadType>::Button(id_type id,
                                 float renderScale,
                                 const std::shared_ptr<sf::RenderWindow>& window,
                                 const std::shared_ptr<ButtonListener<TPayloadType>>& listener)
                                 : Drawable(id, renderScale, window) {
        this->listener = listener;
    }

    template<typename TPayloadType>
    void Button<TPayloadType>::update() {
        Drawable::update();
    }

    template<typename TPayloadType>
    void Button<TPayloadType>::draw() {
        Drawable::draw();
    }

    template<typename TPayloadType>
    void Button<TPayloadType>::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);
    }

    template<typename TPayloadType>
    void Button<TPayloadType>::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);

        if (this->enabled && event.button == sf::Mouse::Button::Left && this->isMouseOver(event.x, event.y))
            this->listener->buttonClicked(parent, id, payload);
    }

    template<typename TPayloadType>
    TPayloadType Button<TPayloadType>::getPayload() const {
        return this->payload;
    }

    template<typename TPayloadType>
    void Button<TPayloadType>::setPayload(const TPayloadType& newPayload) {
        this->payload = newPayload;
    }

}
