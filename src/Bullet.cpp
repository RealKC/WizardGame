#include "Bullet.h"
#include "Utils.h"

#include <cmath>

namespace WizardGame {

Bullet::Bullet(Collider collider)
    : m_collider(collider)
    , m_distance(0)
    , m_angle(0.0f)
{
}

Bullet::~Bullet()
{
}

Bullet Bullet::radial(Vec2 starting_position, Size size, int distance, float angle)
{
    auto bullet = Bullet { Collider { starting_position.x, starting_position.y, size.width, size.height } };
    bullet.m_distance = distance;
    bullet.m_angle = angle;

    return bullet;
}

Bullet Bullet::liniar(Vec2 starting_position, Size size, Direction direction)
{
    float angle = 0;
    switch (direction) {
    case Direction::Down:
        angle = M_PI;
        break;
    case Direction::Up:
        angle = M_PI_2;
        break;
    case Direction::Left:
        angle = 3 * M_PI_2;
        break;
    case Direction::Right:
        angle = 0;
        break;
    }
    return radial(starting_position, size, 3, angle);
}

void Bullet::move()
{
    auto old_x = m_collider.x();
    auto old_y = m_collider.y();

    auto new_radius = 0.1 + m_distance;
    auto new_x = new_radius * cos(m_angle);
    auto new_y = new_radius * sin(m_angle);

    info() << new_x - old_x << ' ' << new_y << std::endl;

    m_collider.apply_position_delta(new_x - old_x, new_y - old_y);
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
