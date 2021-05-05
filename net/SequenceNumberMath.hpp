#pragma once


#include <cstdint>

#define MAX_SEQUENCE_NUMBER 65535 /* = 2^16-1 - 16-битное кодирование номеров пакетов (sequence number) */
#define SEQUENCE_NUMBER_WRAP_AROUND_THRESHOLD 32767 /* = 2^15-1 - половина максимума ^ (выше) */


namespace awd::net {

    class SequenceNumberMath {
    public:
        // Возвращаем true, если первый входной номер пакета (s1) "больше" ("новее") второго (s2).
        static bool isMoreRecent(uint32_t s1, uint32_t s2);

        // Эта операция ассоциативна (входные номера пакетов МОЖНО поменять местами).
        static uint32_t add(uint32_t s1, uint32_t s2);

        // Эта операция НЕ ассоциативна (входные номера пакетов НЕЛЬЗЯ поменять местами).
        static uint32_t subtract(uint32_t minuendSeq, uint32_t subtrahendSeq);
    };

}

