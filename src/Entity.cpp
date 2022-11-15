#include "Entity.h"
#include "Bullet.h"
#include "Utils.h"

namespace WizardGame {

Entity::Entity(Collider collider)
    : m_collider(collider)
{
}

Entity::~Entity()
{
}

void Entity::move_by(int delta_x, int delta_y)
{
    m_collider.apply_position_delta(delta_x, delta_y);
}

Vec2 Entity::position_for_bullet(Size bullet_size, Direction direction) const
{
    switch (direction) {
    case Direction::Up:
        return Vec2 { position().x + size().width / 2 - bullet_size.width / 2, position().y - bullet_size.height };
    case Direction::Right:
        return Vec2 { position().x + size().width / 2 + bullet_size.width / 2, position().y + size().height / 2 };
    case Direction::Left:
        return Vec2 { position().x - bullet_size.width / 2, position().y + size().height / 2 - bullet_size.height / 2 };
    case Direction::Down:
        return Vec2 { position().x + size().width / 2 - bullet_size.width / 2, position().y + size().height };
    }

    error() << "Unknown direction: " << (int)direction << std::endl;
    abort();
}

bool Entity::collides_with(Bullet const& bullet) const
{
    return m_collider.check_collision_with(bullet.collider());
}

bool Entity::collides_with(Entity const& entity) const
{
    return m_collider.check_collision_with(entity.m_collider);
}

void Entity::move_to(int x, int y)
{
    m_collider.move_to(x, y);
}

}
