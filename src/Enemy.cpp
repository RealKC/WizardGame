#include "Enemy.h"

#include "Enemies/Adrian.h"
#include "Enemies/Basic.h"
#include "Utils.h"

namespace WizardGame {

Enemy::Enemy(Collider collider, Vec2 target_position, std::vector<Enemies::Attack> attacks)
    : Entity(collider)
    , m_attacks(attacks)
    , m_target_position(target_position)
    , m_movement_cooldown(0)
{
}

void Enemy::move_to_target_position()
{
    if (m_target_position == position())
        return;

    if (m_movement_cooldown == 0) {
        m_movement_cooldown = 3;
        move_by(0, -1);
        return;
    }

    --m_movement_cooldown;
}

void Enemy::fire_attacks(uint32_t current_time, std::vector<Bullet>& bullets)
{
    for (auto& attack : m_attacks) {
        if (current_time - attack.last_fired_at >= attack.cooldown) {
            attack.last_fired_at = current_time;
            using Enemies::Attack;
            switch (attack.type) {
            case Attack::Type::Circle:
                info() << "Circle\n";
                break;
            case Attack::Type::Line:
                info() << "line\n";
                bullets.push_back(Bullet::liniar(position_for_bullet({ 40, 40 }, Direction::Down), { 40, 40 }, Direction::Down));
                break;
            case Attack::Type::ThreeAtOnce:
                info() << "three\n";
                break;
            }
        }
    }
}

}
