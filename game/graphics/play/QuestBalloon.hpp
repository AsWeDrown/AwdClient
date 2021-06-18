#pragma once


#include "../common/Drawable.hpp"
#include "../common/RichText.hpp"
#include "Quest.hpp"

namespace awd::game {

    class QuestBalloon : public Drawable {
    private:
        float totalTopMargin;
        std::shared_ptr<Quest> quest;
        uint32_t progress = 9999; // см. конструктор и updateProgress()

        std::unique_ptr<sf ::RectangleShape> rect = nullptr;
        std::unique_ptr<sfe::RichText      > text = nullptr;

        void relocate();

    public:
        QuestBalloon(id_type id,
                    float height,
                    float totalTopMargin,
                    const std::shared_ptr<Quest>& quest);

        void update() override;
        void draw() override;

        uint32_t getProgress() const;
        void updateProgress(uint32_t newProgress);
    };

}

