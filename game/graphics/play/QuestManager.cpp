#define FIRST_BALLOON_TOP_MARGIN 250.0f
#define BALLOONS_TOP_MARGIN 40.0f
#define BALLOONS_HEIGHT 150.0f


#include <iostream>
#include "QuestManager.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    id_type QuestManager::questIdToDrawableId(uint32_t questId) {
        return ID_SCREEN_PLAY_QUEST_MANAGER_QUEST_BALLOON + questId;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    QuestManager::QuestManager() : Drawable(ID_SCREEN_PLAY_QUEST_MANAGER) {
        totalTopMargin    = FIRST_BALLOON_TOP_MARGIN * renderScale;
        balloonsTopMargin = BALLOONS_TOP_MARGIN      * renderScale;
        balloonsHeight    = BALLOONS_HEIGHT          * renderScale;
    }

    void QuestManager::update() {
        Drawable::update();
    }

    void QuestManager::draw() {
        Drawable::draw();
    }

    bool QuestManager::isQuestActive(uint32_t questId) const {
        return std::any_of(activeQuests.cbegin(), activeQuests.cend(),
                [questId](const std::shared_ptr<Quest>& quest)
                        { return quest->getId() == questId; });
    }

    void QuestManager::beginQuest(uint32_t questId, uint32_t questType, uint32_t maxQuestProgress) {
        if (isQuestActive(questId))
            return;

        totalTopMargin += balloonsHeight + balloonsTopMargin;
        auto quest = std::make_shared<Quest>(questId, questType, maxQuestProgress);
        activeQuests.push_back(quest);

        auto balloon = std::make_shared<QuestBalloon>(
                questIdToDrawableId(questId), balloonsHeight, totalTopMargin, quest);

        enqueueAddChild(balloon);
    }

    void QuestManager::advanceQuest(uint32_t questId, uint32_t newQuestProgress) {
        if (!isQuestActive(questId))
            return;

        auto balloon = std::dynamic_pointer_cast
                <QuestBalloon>(getChildById(questIdToDrawableId(questId)));

        balloon->updateProgress(newQuestProgress);
    }

    void QuestManager::endQuest(uint32_t questId, uint32_t status) {
        if (!isQuestActive(questId))
            return;

        totalTopMargin -= balloonsHeight + balloonsTopMargin;
        activeQuests.erase(std::remove_if(
                activeQuests.begin(), activeQuests.end(),
                [questId](const std::shared_ptr<Quest>& quest) {
                    return quest->getId() == questId;
                }), activeQuests.end()
        );

        enqueueRemoveChild(questIdToDrawableId(questId));

        switch (status) {
            default:
                std::wcerr << L"Unhandled quest end status: " << status << std::endl;
                break;

            case Quest::ENDED_COMPLETE_FULL:
                Game::instance().getSoundSystem()->playSound(Sound::QUEST_COMPLETED);
                break;
        }
    }

}

