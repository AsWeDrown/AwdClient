#pragma once


#include <string>

namespace awd::game {

    class Quest {
    private:
        uint32_t id, type, maxProgress;
        std::wstring desc;

        // TODO сделать нормально
        static std::wstring getTypeDesc(uint32_t questType);

    public:
        Quest(uint32_t id, uint32_t type, uint32_t maxProgress);

        uint32_t     getId         () const;
        uint32_t     getType       () const;
        uint32_t     getMaxProgress() const;
        std::wstring getDesc       () const;
    };

}

