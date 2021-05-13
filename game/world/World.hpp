#pragma once


#include "../graphics/common/Drawable.hpp"
#include "WorldData.hpp"

namespace awd::game {

    class World : public Drawable {
    private:
        uint32_t dimension = 9999; // 9999 -> ничего не рисуем (мир ещё не загружен)

        std::shared_ptr<WorldData> worldData = nullptr;

        std::unique_ptr<sf::VertexArray> worldVertices = nullptr;

    public:
        World();

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        void focusCamera(float worldX, float worldY);
        void centerCamera();
        void updateDimension(uint32_t newDimension);
    };

}

