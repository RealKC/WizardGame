#include "Bullet.h"
#include "Utils.h"

#include <cmath>

namespace WizardGame {

Bullet::Bullet(Collider collider, Type type)
    : m_collider(collider)
    , m_type(type)
    , m_distance(0)
    , m_angle(0.0f)
{
}

Bullet::~Bullet()
{
}

Bullet Bullet::vertical(WizardGame::Vec2 starting_position, WizardGame::Size size)
{
    return Bullet { Collider { starting_position.x, starting_position.y, size.width, size.height }, Type::Vertical };
}

Bullet Bullet::horizontal(WizardGame::Vec2 starting_position, WizardGame::Size size)
{
    return Bullet { Collider { starting_position.x, starting_position.y, size.width, size.height }, Type::Horizontal };
}
Bullet Bullet::radial(Vec2 starting_position, Size size, int distance, float angle)
{
    auto bullet = Bullet { Collider { starting_position.x, starting_position.y, size.width, size.height }, Type::Radial };
    bullet.m_distance = distance;
    bullet.m_angle = angle;

    return bullet;
}

void Bullet::move()
{
    switch (m_type) {
    case Type::Radial: {
        auto old_x = m_collider.x();
        auto old_y = m_collider.y();

        auto new_radius = 0.1 + m_distance;
        auto new_x = new_radius * cos(m_angle);
        auto new_y = new_radius * sin(m_angle);

        info() << new_x - old_x << ' ' << new_y << std::endl;

        m_collider.apply_position_delta(new_x - old_x, new_y - old_y);
        break;
    }
    case Type::Horizontal:
        m_collider.apply_position_delta(1, 0);
        break;
    case Type::Vertical:
        m_collider.apply_position_delta(0, 1);
        break;
    }
}

Vec2 Bullet::position() const
{
    return Vec2 { m_collider.x(), m_collider.y() };
}

Size Bullet::size() const
{
    return Size { m_collider.w(), m_collider.h() };
}

}
