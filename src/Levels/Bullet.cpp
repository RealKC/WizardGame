#include "Bullet.h"
#include "../Utils.h"

#define _USE_MATH_DEFINES // For constants like M_PI etc
#include <cmath>

namespace WizardGame {

Bullet::Bullet(Collider collider, float speed)
    : m_collider(collider)
    , m_distance(0)
    , m_angle(0.0f)
    , m_speed(speed)
{
}

Bullet::~Bullet()
{
}

Bullet Bullet::radial(Vec2 starting_position, Size size, int distance, float angle, float speed)
{
    auto bullet = Bullet { Collider { starting_position.x, starting_position.y, size.width, size.height }, speed };
    bullet.m_distance = distance;
    bullet.m_angle = angle;
    bullet.m_origin = starting_position;

    return bullet;
}

Bullet Bullet::liniar(Vec2 starting_position, Size size, Direction direction, float speed)
{
    float angle = 0;
    switch (direction) {
    case Direction::Down:
        angle = M_PI_2;
        break;
    case Direction::Up:
        angle = 3 * M_PI_2;
        break;
    case Direction::Left:
        angle = M_PI;
        break;
    case Direction::Right:
        angle = 0;
        break;
    }
    return radial(starting_position, size, 3, angle, speed);
}

HasHitWall Bullet::move()
{
    auto new_radius = m_speed + m_distance;
    auto new_x = m_origin.x + new_radius * cos(m_angle);
    auto new_y = m_origin.y + new_radius * sin(m_angle);

    m_distance = new_radius;

    return m_collider.move_to(new_x, new_y);
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
