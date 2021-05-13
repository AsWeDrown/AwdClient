#pragma once


#include <memory>
#include <SFML/Graphics.hpp>
#include "TileBlock.hpp"

namespace awd::game {

    class WorldData {
    public:
        uint32_t dimension, // ID измерения (отсчёт с единцы!)
                 width,     // ширина всего мира, в тайлах
                 height,    // высота всего мира, в тайлах
                 tileSize;  // ширина и высота всех тайлов

        std::vector<std::shared_ptr<TileBlock>> tiles;

        std::shared_ptr<sf::VertexArray> worldVertices; // Vertex'ы (наборы Quad'ов), отвечающие за прорисовку мира
    };

}
