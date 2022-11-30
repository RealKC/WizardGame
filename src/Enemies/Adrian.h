#pragma once

#include "../Enemy.h"

namespace WizardGame {
namespace Enemies {

class Adrian : public Enemy {
public:
    Adrian(Collider collider, Vec2 target_position, int phase)
        : Enemy(collider, target_position)
        , m_phase(phase)
    {
    }

    // ^Enemy
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) override;

    // ^Entity
    virtual void render(SDL_Renderer*) override { }

private:
    int m_phase;
};

} // WizardGame
} // Enemies
