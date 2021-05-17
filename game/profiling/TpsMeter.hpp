#pragma once


#include <memory>
#include <mutex>
#include <deque>

namespace awd::game {

    typedef std::chrono::steady_clock
            game_clock;

    typedef std::chrono::time_point<game_clock, std::chrono::nanoseconds>
            game_time;

    /**
     * Измеритель показателя TPS (ticks per second).
     *
     * Принцип работы:
     *
     *     1) в начале каждого цикла обновления (тика) вызывается метод onUpdate();
     *
     *     2) измеритель записывает задержку между этим тиком и предыдущим в специальную очередь;
     *
     *     3) с помощью метода estimateTps() можно в любом момент получить текущий показатель TPS^
     *        этот показатель основан на среднем арифметическом взвешенном всех задержек между тиками,
     *        в момент вызова хранящихся в специальной очереди - чем старее была сделана запись о
     *        конкретной задержке, тем меньше этот экземпляр повлияет на итоговый результат (TPS),
     *        т.е. сильнее всего на результат (TPS) влияют самые недавние показатели задержки;
     *
     *     4) метод estimateTps() может вернуть отрицательный TPS в случае, если он был вызван слишком
     *        рано (измеритель ещё не успел собрать достаточное число экземпляров для качественной оценки).
     *
     * Не рекомендуется вызывать метод estimateTps() часто - он может работать довольно медленно.
     */
    class TpsMeter {
    private:
        std::mutex               mutex;
        uint32_t                 samplesNum;
        std::deque<float>        recentTickDelays;
        std::shared_ptr<game_time> lastTickNanoTime = nullptr;

    public:
        explicit TpsMeter(uint32_t gameTps);

        /**
         * Должно выполняться в начале каждого цикла обновления (тика).
         */
        void onUpdate();

        /**
         * Вычисляет текущий показатель TPS на основе хранимых в данный момент измерителем образцов.
         *
         * Возвращаемое значение будет отрицательным, если оно было вычислено приближённо, т.е. если
         * измеритель проработал недостаточно долго с момента его запуска для точного вычисления: в
         * этом случае вызвавший этот метод должен считать текущий TPS, приблизительно (!) равным
         * полученному от этого метода значению, взятому по абсолютной величине.
         */
        float estimateTps();
    };

}

