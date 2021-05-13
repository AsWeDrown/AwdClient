#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    class WorldData {
    public:
        uint32_t dimension, // ID измерения (начинается с 1)
                 width,     // ширина всего мира, в тайлах
                 height,    // высота всего мира, в тайлах
                 tileSize;  // ширина и высота всех тайлов

        std::shared_ptr<sf::VertexArray> worldVetrices; // Vertex'ы (наборы Quad'ов), отвечающие за прорисовку мира
    };

}
