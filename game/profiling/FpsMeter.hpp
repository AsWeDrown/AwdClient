#pragma once


#include "TpsMeter.hpp"

namespace awd::game {

    /**
     * Принцип работы аналогичен измерителю TPS (TpsMeter).
     *
     * Единственное отличие - estimateFps() никогда не возвращает отрицательных значений.
     * Вместо этого он возвращает 0 в случае нехватки данных для достаточно точной оценки.
     */
    class FpsMeter {
    private:
        std::mutex                 mutex;
        std::deque<float>          recentFrameDelays;
        std::shared_ptr<game_time> lastFrameNanoTime = nullptr;

    public:
        void onDraw();
        uint32_t estimateFps();
    };

}

