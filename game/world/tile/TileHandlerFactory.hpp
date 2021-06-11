#pragma once


#include <memory>
#include "SolidHandler.hpp"
#include "VoidHandler.hpp"
#include "LadderHandler.hpp"

namespace awd::game {

    class TileHandlerFactory {
    public:
        virtual std::shared_ptr<TileHandler> newTileHandler() const = 0;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////

    class SolidHandlerFactory : public TileHandlerFactory {
        inline std::shared_ptr<TileHandler> newTileHandler() const final {
            return std::make_shared<SolidHandler>();
        }
    };

    class VoidHandlerFactory : public TileHandlerFactory {
        inline std::shared_ptr<TileHandler> newTileHandler() const final {
            return std::make_shared<VoidHandler>();
        }
    };

    class LadderHandlerFactory : public TileHandlerFactory {
        inline std::shared_ptr<TileHandler> newTileHandler() const final {
            return std::make_shared<LadderHandler>();
        }
    };

}

