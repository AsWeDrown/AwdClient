#include "Quest.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    // TODO сделать нормально
    std::wstring Quest::getTypeDesc(uint32_t questType) {
        switch (questType) {
            case 1: // QuestMoveAround
                return L"Осмотритесь.";

            case 2: // QuestFixElectricity
                return L"Восстановите электричество. Используйте рубильники.";

            case 3: // QuestShatterRoof
                return L"Создайте тряску, чтобы опрокинуть шкаф на 2-ом этаже.";

            case 4: // QuestGetTogether
                return L"Соберитесь вместе и обсудите план действий.";

            case 5: // QuestVisitCaptain
                return L"Доберитесь до капитана подлодки.";

            default: // ???
                return L"< ??? | QuestType: " + std::to_wstring(questType) + L" >";
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Quest::Quest(uint32_t id, uint32_t type, uint32_t maxProgress) {
        this->id = id;
        this->type = type;
        this->maxProgress = maxProgress;
        this->desc = getTypeDesc(type);
    }

    uint32_t Quest::getId() const {
        return id;
    }

    uint32_t Quest::getType() const {
        return type;
    }

    uint32_t Quest::getMaxProgress() const {
        return maxProgress;
    }

    std::wstring Quest::getDesc() const {
        return desc;
    }

}
