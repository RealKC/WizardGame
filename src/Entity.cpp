#include "Entity.h"

namespace WizardGame {

Entity::Entity(Vec2 starting_position)
    : m_position(starting_position)
{
}

Entity::~Entity()
{
}

void Entity::move_by(int delta_x, int delta_y)
{
    m_position.x += delta_x;
    m_position.y += delta_y;
}

}
