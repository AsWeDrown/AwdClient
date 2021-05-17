/**
 * За какой промежуток времени должно измеряться значение TPS.
 */
#define SAMPLING_SECONDS 60

/**
 * Множитель веса очередного образца после каждой итерации.
 */
#define WEIGHT_MOMENTUM 0.9995f

/**
 * Число наносекунд в одной секунде.
 */
#define NANOS_IN_SECOND 1E+9f


#include <iostream>
#include "TpsMeter.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TpsMeter::TpsMeter(uint32_t gameTps) {
        samplesNum = SAMPLING_SECONDS * gameTps;
    }

    void TpsMeter::onUpdate() {
        auto thisTickNanoTime = std::make_shared<nanos_t>(std::chrono::high_resolution_clock::now());

        if (lastTickNanoTime == nullptr)
            // Лениво инициализируем (чтобы не было "гигантских" значений задержки).
            lastTickNanoTime = thisTickNanoTime;

        std::unique_lock<std::mutex> lock(mutex);

        if (recentTickDelays.size() == samplesNum)
            recentTickDelays.pop_front(); // удаляем самый "старый" экземпляр данных

        recentTickDelays.push_back((float) std::chrono::duration_cast
                <std::chrono::nanoseconds>(*thisTickNanoTime - *lastTickNanoTime).count());
        lastTickNanoTime = thisTickNanoTime;
    }

    float TpsMeter::estimateTps() {
        std::unique_lock<std::mutex> lock(mutex);

        float weightedSamplesSum   = 0.0f;
        float weightsSum           = 0.0f;
        float sampleWeight         = 1.0f;

        for (int i = recentTickDelays.size() - 1; i >= 0; i--) { // NOLINT(cppcoreguidelines-narrowing-conversions)
            weightedSamplesSum += sampleWeight * recentTickDelays[i];
            weightsSum         += sampleWeight;
            sampleWeight       *= WEIGHT_MOMENTUM;
        }

        float weightedAverageTickDelay = weightedSamplesSum / weightsSum;
        float tps = NANOS_IN_SECOND / weightedAverageTickDelay;

        return recentTickDelays.size() == samplesNum ? tps : -tps;
    }

}
