#include "FallableLivingEntity.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    FallableLivingEntity::FallableLivingEntity(uint32_t entityType, id_type entityId)
                                               : LivingEntity(entityType, entityId) {}

    uint32_t FallableLivingEntity::getMidairTicks() const {
        return midairTicks;
    }

    float FallableLivingEntity::getFallDistance() const {
        return fallDistance;
    }

}
