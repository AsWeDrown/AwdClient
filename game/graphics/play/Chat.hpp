#pragma once


#include <deque>
#include "../common/Drawable.hpp"
#include "../common/RichText.hpp"

namespace awd::game {

    class Chat : public Drawable {
    private:
        std::wstring wholeChat;

        std::unique_ptr<sf ::RectangleShape> rect = nullptr;
        std::unique_ptr<sfe::RichText      > text = nullptr;

    public:
        Chat();

        void update() override;
        void draw() override;

        void addChatMessage(const std::wstring& message);
    };

}

