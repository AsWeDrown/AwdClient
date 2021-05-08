#include <algorithm>
#include "SequenceNumberMath.hpp"

namespace awd::net {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool SequenceNumberMath::isMoreRecent(uint32_t s1, uint32_t s2) {
        // Возвращаем true, если первый входной номер пакета (s1) "больше" ("новее") второго (s2).
        return ((s1 > s2) && (s1 - s2 <= SEQUENCE_NUMBER_WRAP_AROUND_THRESHOLD)) // s1 просто "новее" s2
            || ((s1 < s2) && (s2 - s1 >  SEQUENCE_NUMBER_WRAP_AROUND_THRESHOLD)); // s1 "новее" из-за wrap-around'а
    }

    uint32_t SequenceNumberMath::add(uint32_t s1, uint32_t s2) {
        // Эта операция ассоциативна (входные номера пакетов МОЖНО поменять местами).
        uint32_t biggerSeq    = std::max(s1, s2);
        uint32_t smallerSeq   = std::min(s1, s2);
        uint32_t maxDirectAdd = MAX_SEQUENCE_NUMBER - biggerSeq;

        return smallerSeq <= maxDirectAdd
               ? biggerSeq + smallerSeq // прямое сложение (влезает в верхний предел)
               : smallerSeq - maxDirectAdd - 1; // wrap-around
    }

    uint32_t SequenceNumberMath::subtract(uint32_t minuendSeq, uint32_t subtrahendSeq) {
        // Эта операция НЕ ассоциативна (входные номера пакетов НЕЛЬЗЯ поменять местами).
        return minuendSeq >= subtrahendSeq
               ? minuendSeq - subtrahendSeq // прямое вычитание (влезает в нижний предел)
               : MAX_SEQUENCE_NUMBER - (subtrahendSeq - minuendSeq) + 1; // wrap-around
    }

}
