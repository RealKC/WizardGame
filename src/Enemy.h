#pragma once

#include "Entity.h"
#include "Vec2.h"
#include <memory>
#include <stdint.h>

namespace WizardGame {

class Enemy : public Entity {
public:
    enum class Type {
        Basic = 0,
        Adrian1,
        Adrian2,
    };

    virtual void tick(uint32_t current_time) = 0;

protected:
    Enemy(Collider collider)
        : Entity(collider)
    {
    }
};

std::unique_ptr<Enemy> make_enemy(Vec2 starting_position, Enemy::Type = Enemy::Type::Basic);

class BasicEnemy : public Enemy {
public:
    // ^Enemy
    virtual void tick(uint32_t current_time) override;

    // ^Entity
    virtual void render(SDL_Renderer*) override {}

private:
    friend std::unique_ptr<Enemy> make_enemy(Vec2, Type);

    BasicEnemy(Collider collider)
        : Enemy(collider)
    {
    }
};

class AdrianEnemy : public Enemy {
public:
    // ^Enemy
    virtual void tick(uint32_t current_time) override;

    // ^Entity
    virtual void render(SDL_Renderer*) override {}

private:
    friend std::unique_ptr<Enemy> make_enemy(Vec2, Type);

    AdrianEnemy(Collider collider, int phase)
        : Enemy(collider)
        , m_phase(phase)
    {
    }

    int m_phase;
};

} // WizardGame
