#include "RectWrapper.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    RectWrapper::RectWrapper(unsigned int preservedWidth, unsigned int preservedHeight,
                             const std::shared_ptr<sf::RectangleShape>& rect) {
        this->preservedWidth = preservedWidth;
        this->preservedHeight = preservedHeight;
        this->rect = rect;
    }

    unsigned int RectWrapper::getPreservedWidth() const {
        return preservedWidth;
    }

    unsigned int RectWrapper::getPreservedHeight() const {
        return preservedHeight;
    }

    std::shared_ptr<sf::RectangleShape> RectWrapper::getRect() const {
        return rect;
    }

}
