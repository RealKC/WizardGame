#pragma once

#include "../Vec2.h"
#include "Bullet.h"
#include "Enemies/Attack.h"
#include "Entity.h"
#include <memory>
#include <stdint.h>
#include <vector>

namespace WizardGame {

class Enemy : public Entity {
public:
    enum class Died {
        Yes,
        No,
    };

    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) = 0;
    virtual Died hit() = 0;

protected:
    Enemy(Collider collider, Vec2 target_position, std::vector<Enemies::Attack> attacks);

    /// Enemies spawn outside the screen and go towards a specific position on the screen
    /// This function implements the logic for that, but it still needs to be called manually
    /// in subclasses.
    void move_to_target_position();

    void fire_attacks(uint32_t current_time, std::vector<Bullet>& bullets);

    std::vector<Enemies::Attack> m_attacks;
    void reset_current_attack() { m_current_attack = 0; }

private:
    std::size_t m_current_attack;

    Vec2 m_target_position;
    int m_movement_cooldown;
};

} // WizardGame
