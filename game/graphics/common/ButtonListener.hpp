#pragma once


#include "Drawable.hpp"

namespace awd::game {

    template<typename TPayloadType>
    class ButtonListener {
    public:
        virtual void buttonClicked(Drawable* buttonParent, id_type buttonId,
                                   const TPayloadType& payload) = 0;
    };

}

