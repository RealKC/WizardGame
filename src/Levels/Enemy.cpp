#include "Enemy.h"

#include "../Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace WizardGame {

static constexpr int STEP = 1;

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
        m_movement_cooldown = 1;
        int delta_x = 0;
        int delta_y = 0;

        if (position().x < m_target_position.x) {
            delta_x = STEP;
        } else if (position().x > m_target_position.x) {
            delta_x = -STEP;
        }

        if (position().y < m_target_position.y) {
            delta_y = STEP;
        } else if (position().y > m_target_position.y) {
            delta_y = -STEP;
        }

        move_by(delta_x, delta_y);
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
            case Attack::Type::Circle: {
                float angle = M_PI / 5;
                for (int i = 1; i < 6; ++i) {
                    bullets.push_back(Bullet::radial(position_for_bullet({ 20, 20 }, Direction::Left), { 20, 20 }, 10, angle * i, 2));
                }
                break;
            }
            case Attack::Type::Line:
                bullets.push_back(Bullet::liniar(position_for_bullet({ 10, 10 }, Direction::Down), { 10, 10 }, Direction::Down, 2));
                break;
            case Attack::Type::ThreeAtOnce:
                bullets.push_back(Bullet::liniar(position_for_bullet({ 10, 10 }, Direction::Down) - Vec2 { 10, 0 }, { 10, 10 }, Direction::Down, 2));
                bullets.push_back(Bullet::liniar(position_for_bullet({ 10, 10 }, Direction::Down), { 10, 10 }, Direction::Down, 2));
                bullets.push_back(Bullet::liniar(position_for_bullet({ 10, 10 }, Direction::Down) + Vec2 { 10, 0 }, { 10, 10 }, Direction::Down, 2));

                break;
            }
        }
    }
}

}
