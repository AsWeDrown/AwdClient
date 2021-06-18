/**
 * Храним информацию о задержках между последними 60 кадрами.
 */
#define FPS_SAMPLES_NUM 60

/**
 * Множитель веса очередного образца после каждой итерации.
 */
#define FPS_WEIGHT_MOMENTUM 0.9905f

/**
 * Число наносекунд в одной секунде.
 */
#define FPS_NANOS_IN_SECOND 1E+9f


#include <cmath>
#include "FpsMeter.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void FpsMeter::onDraw() {
        auto thisFrameNanoTime = std::make_shared<game_time>(game_clock::now());

        if (lastFrameNanoTime == nullptr)
            // Лениво инициализируем (чтобы не было "гигантских" значений задержки).
            lastFrameNanoTime = thisFrameNanoTime;

        std::unique_lock<std::mutex> lock(mutex);

        if (recentFrameDelays.size() == FPS_SAMPLES_NUM)
            recentFrameDelays.pop_front(); // удаляем самый "старый" экземпляр данных

        recentFrameDelays.push_back(static_cast<float>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                        *thisFrameNanoTime - *lastFrameNanoTime).count()));

        lastFrameNanoTime = thisFrameNanoTime;
    }

    uint32_t FpsMeter::estimateFps() {
        std::unique_lock<std::mutex> lock(mutex);

        if (recentFrameDelays.size() < FPS_SAMPLES_NUM)
            return 0.0f;

        float weightedSamplesSum   = 0.0f;
        float weightsSum           = 0.0f;
        float sampleWeight         = 1.0f;

        for (int i = recentFrameDelays.size() - 1; i >= 0; i--) { // NOLINT(cppcoreguidelines-narrowing-conversions)
            weightedSamplesSum += sampleWeight * recentFrameDelays[i];
            weightsSum         += sampleWeight;
            sampleWeight       *= FPS_WEIGHT_MOMENTUM;
        }

        float weightedAverageTickDelay = weightedSamplesSum / weightsSum;
        float fps = FPS_NANOS_IN_SECOND / weightedAverageTickDelay;

        return std::floor(fps);
    }

}
