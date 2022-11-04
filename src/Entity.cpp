#include "Entity.h"

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

}
