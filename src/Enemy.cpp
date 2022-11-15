#include "Enemy.h"

#include "Enemies/Adrian.h"
#include "Enemies/Basic.h"
#include "Utils.h"

namespace WizardGame {

Enemy::Enemy(Collider collider, Vec2 target_position)
    : Entity(collider)
    , m_target_position(target_position)
    , m_cooldown(0)
{
}

std::unique_ptr<Enemy> make_enemy(Vec2 starting_position, Vec2 target_position, Enemy::Type type)
{
    switch (type) {
    case Enemy::Type::Basic:
        return std::unique_ptr<Enemy>(new Enemies::Basic(Collider(starting_position.x, starting_position.y, 50, 50), target_position));
    case Enemy::Type::Adrian1:
        return std::unique_ptr<Enemy>(new Enemies::Adrian(Collider(0, 0, 0, 0), target_position, 1));
    case Enemy::Type::Adrian2:
        return std::unique_ptr<Enemy>(new Enemies::Adrian(Collider(0, 0, 0, 0), target_position, 2));
    default:
        error() << "Unknown enemy type: " << (int)type << "\n";
    }
}

void Enemy::move_to_target_position()
{
    if (m_target_position == position())
        return;

    if (m_cooldown == 0) {
        m_cooldown = 3;
        move_by(0, -1);
        return;
    }

    --m_cooldown;
}

}
