#pragma once

#include "Entity.h"
#include "Vec2.h"
#include <memory>

namespace WizardGame {

class Enemy : public Entity {
public:
    enum class Type {
        Basic,
        Adrian1,
        Adrian2,
    };

    std::unique_ptr<Enemy> spawn(Vec2 starting_position, Type = Type::Basic);

    virtual void tick() = 0;

protected:
    Enemy(Collider collider)
        : Entity(collider)
    {
    }
};

class BasicEnemy : public Enemy {
public:
    virtual void tick() override;

private:
    friend std::unique_ptr<Enemy> Enemy::spawn(Vec2, Type);

    BasicEnemy(Collider collider)
        : Enemy(collider)
    {
    }
};

class AdrianEnemy : public Enemy {
public:
    virtual void tick() override;

private:
    friend std::unique_ptr<Enemy> Enemy::spawn(Vec2, Type);

    AdrianEnemy(Collider collider, int phase)
        : Enemy(collider)
        , m_phase(phase)
    {
    }

    int m_phase;
};

} // WizardGame
