#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class LoadingOverlay : public Drawable {
    private:
        std::wstring unformattedMessage;

        uint32_t existedTicks = 0;
        bool reverseTick = false;

        std::unique_ptr<sf::Text> text = nullptr;

    public:
        LoadingOverlay(id_type id,
                       const std::wstring& message);

        void update() override;
        void draw() override;

        std::wstring getUnformattedMessage() const;
    };

}

