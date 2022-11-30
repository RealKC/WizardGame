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
