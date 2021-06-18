#pragma once


#include "QuestBalloon.hpp"
#include "../common/Drawable.hpp"

namespace awd::game {

    class QuestManager : public Drawable {
    private:
        std::vector<std::shared_ptr<Quest>> activeQuests;

        float totalTopMargin;
        float balloonsTopMargin;
        float balloonsHeight;

        static id_type questIdToDrawableId(uint32_t questId);

    public:
        QuestManager();

        void update() override;
        void draw() override;

        bool isQuestActive(uint32_t questId) const;
        void beginQuest(uint32_t questId, uint32_t questType, uint32_t maxQuestProgress);
        void advanceQuest(uint32_t questId, uint32_t newQuestProgress);
        void endQuest(uint32_t questId, uint32_t status);
    };

}

