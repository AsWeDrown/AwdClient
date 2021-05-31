#pragma once


#include "LivingEntity.hpp"
#include "Fallable.hpp"

namespace awd::game {

    class FallableLivingEntity :
            public LivingEntity,
            public Fallable {
    protected:
        uint32_t midairTicks          =    0;
        float    lastTickFallDistance = 0.0f,
                 fallDistance         = 0.0f;

    public:
        FallableLivingEntity(uint32_t entityType, id_type entityId);

        uint32_t getMidairTicks () const override;
        float    getFallDistance() const override;
    };

}
