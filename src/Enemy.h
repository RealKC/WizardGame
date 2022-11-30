#pragma once

#include "Bullet.h"
#include "Entity.h"
#include "Vec2.h"
#include <memory>
#include <stdint.h>
#include <vector>

namespace WizardGame {

class Enemy : public Entity {
public:
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) = 0;

protected:
    Enemy(Collider collider, Vec2 target_position);

    /// Enemies spawn outside the screen and go towards a specific position on the screen
    /// This function implements the logic for that, but it still needs to be called manually
    /// in subclasses.
    void move_to_target_position();

private:
    Vec2 m_target_position;
    int m_cooldown;
};

} // WizardGame
