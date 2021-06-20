#pragma once


#include <memory>
#include "SolidHandler.hpp"
#include "VoidHandler.hpp"
#include "LadderHandler.hpp"
#include "SwitchHandler.hpp"
#include "HatchOpenHandler.hpp"
#include "HatchClosedHandler.hpp"

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

    class SwitchHandlerFactory : public TileHandlerFactory {
        inline std::shared_ptr<TileHandler> newTileHandler() const final {
            return std::make_shared<SwitchHandler>();
        }
    };

    class HatchOpenHandlerFactory : public TileHandlerFactory {
        inline std::shared_ptr<TileHandler> newTileHandler() const final {
            return std::make_shared<HatchOpenHandler>();
        }
    };

    class HatchClosedHandlerFactory : public TileHandlerFactory {
        inline std::shared_ptr<TileHandler> newTileHandler() const final {
            return std::make_shared<HatchClosedHandler>();
        }
    };

}

