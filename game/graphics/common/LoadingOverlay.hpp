#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class LoadingOverlay : public Drawable {
    private:
        std::wstring unformattedMessage;

        unsigned int existedTicks = 0;
        bool reverseTick = false;

        std::unique_ptr<sf::Text> text = nullptr;

    public:
        LoadingOverlay(id_type id,
                       float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window,
                       const std::wstring& message);

        void update() override;
        void draw() override;

        std::wstring getUnformattedMessage() const;
    };

}

