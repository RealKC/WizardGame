#pragma once

#include "../../Utils.h"
#include "../Enemy.h"

namespace WizardGame {
namespace Enemies {

class Adrian : public Enemy {
public:
    Adrian(Collider collider, Vec2 target_position, std::vector<Attack> attacks, int health, int score)
        : Enemy(collider, target_position, attacks)
        , m_health(health)
        , m_score(score)
    {
    }

    // ^Enemy
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) override;
    virtual Died hit() override;

    // ^Entity
    virtual void render(SDL_Renderer*, SpriteManager&) override;
    virtual std::int64_t score_value() const override { return m_score * m_health / (100 + m_score); }

private:
    int m_health;
    int m_score;
};

} // WizardGame
} // Enemies
