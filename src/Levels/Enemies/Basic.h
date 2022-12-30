#pragma once

#include "../Enemy.h"

namespace WizardGame {
namespace Enemies {

class Basic : public Enemy {
public:
    Basic(Collider collider, Vec2 target_position, std::vector<Attack> attacks);

    // ^Enemy
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) override;
    virtual Died hit() override { return Died::Yes; }

    // ^Entity
    virtual std::int64_t score_value() const override { return 20; }

protected:
    // ^Entity
    virtual SpriteId sprite_id() const override { return m_sprite_id; }

private:
    SpriteId m_sprite_id;
};

}
}
