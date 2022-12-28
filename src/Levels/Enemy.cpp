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
    constexpr Size BULLET_SIZE { 10, 10 };
    for (auto& attack : m_attacks) {
        if (current_time - attack.last_fired_at >= attack.cooldown) {
            attack.last_fired_at = current_time;
            using Enemies::Attack;
            switch (attack.type) {
            case Attack::Type::Circle: {
                float angle = 0;
                for (int i = 1; i <= 6; ++i) {
                    auto direction = i < 3 ? Direction::Right : (i < 5 ? Direction::Down : Direction::Left);
                    bullets.push_back(Bullet::radial(position_for_bullet(BULLET_SIZE, direction), BULLET_SIZE, 10, angle, 2));
                    angle += M_PI / 5;
                }
                break;
            }
            case Attack::Type::Line:
                bullets.push_back(Bullet::liniar(position_for_bullet(BULLET_SIZE, Direction::Down), BULLET_SIZE, Direction::Down, 2));
                break;
            case Attack::Type::ThreeAtOnce: {
                auto position = position_for_bullet(BULLET_SIZE, Direction::Down);

                bullets.push_back(Bullet::liniar(position - Vec2 { 50, 0 }, BULLET_SIZE, Direction::Down, 2));
                bullets.push_back(Bullet::liniar(position, { 10, 10 }, Direction::Down, 2));
                bullets.push_back(Bullet::liniar(position + Vec2 { 50, 0 }, BULLET_SIZE, Direction::Down, 2));

                break;
            }
            }
        }
    }
}

}
