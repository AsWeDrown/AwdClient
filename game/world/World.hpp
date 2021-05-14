#pragma once


#include "../graphics/common/Drawable.hpp"
#include "WorldData.hpp"
#include "../entity/Entity.hpp"

namespace awd::game {

    class World : public Drawable {
    private:
        uint32_t dimension = 9999; // 9999 -> ничего не рисуем (мир ещё не загружен)

        std::shared_ptr<WorldData> worldData = nullptr;
        std::vector<std::shared_ptr<Entity>> entities;

    public:
        World();

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        void focusCamera(float worldX, float worldY);
        void centerCamera();

        void updateDimension(uint32_t newDimension);

        std::shared_ptr<Entity> getEntityById(uint32_t entityId) const;
        void addEntity(const std::shared_ptr<Entity>& entity);
        void removeEntity(uint32_t entityId);
    };

}

