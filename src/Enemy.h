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
    enum class Type {
        Basic = 0,
        Adrian1,
        Adrian2,
    };

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

std::unique_ptr<Enemy> make_enemy(Vec2 starting_position, Vec2 target_position, Enemy::Type = Enemy::Type::Basic);

class BasicEnemy : public Enemy {
public:
    // ^Enemy
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) override;

    // ^Entity
    virtual void render(SDL_Renderer*) override;

private:
    friend std::unique_ptr<Enemy> make_enemy(Vec2, Vec2, Type);

    BasicEnemy(Collider collider, Vec2 target_position)
        : Enemy(collider, target_position)
    {
    }
};

class AdrianEnemy : public Enemy {
public:
    // ^Enemy
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) override;

    // ^Entity
    virtual void render(SDL_Renderer*) override { }

private:
    friend std::unique_ptr<Enemy> make_enemy(Vec2, Vec2, Type);

    AdrianEnemy(Collider collider, Vec2 target_position, int phase)
        : Enemy(collider, target_position)
        , m_phase(phase)
    {
    }

    int m_phase;
};

} // WizardGame
