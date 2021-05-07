#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class LoadingOverlay : public Drawable {
    private:
        sf::Text text;
        unsigned int existedTicks = 0;

    public:
        LoadingOverlay(id_type id,
                       float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window,
                       const std::wstring& message);

        void update() override;
        void draw() override;
    };

}

