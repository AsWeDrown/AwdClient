#pragma once


#include "Drawable.hpp"

namespace awd::game {

    class TextFieldListener {
    public:
        virtual void contentsChanged(Drawable* textFieldParent, id_type textFieldId,
                                     const std::wstring& newContents) = 0;
    };

}

