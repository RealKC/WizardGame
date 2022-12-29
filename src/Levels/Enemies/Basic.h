#pragma once

#include "../Enemy.h"

namespace WizardGame {
namespace Enemies {

class Basic : public Enemy {
public:
    Basic(Collider collider, Vec2 target_position, std::vector<Attack> attacks)
        : Enemy(collider, target_position, attacks)
    {
    }

    // ^Enemy
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) override;
    virtual Died hit() override { return Died::Yes; }

    // ^Entity
    virtual void render(SDL_Renderer*, SpriteManager&) override;
    virtual std::int64_t score_value() const override { return 20; }
};

}
}
