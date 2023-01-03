#include "Entity.h"

#include "../Utils.h"
#include "Bullet.h"

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

void Entity::render([[maybe_unused]] SDL_Renderer* renderer, SpriteManager& sprite_manager)
{
    // We make sure the entity's sprite has the same center as its hitbox
    auto position = this->position();
    auto sprite_size = SpriteManager::size_for_sprite_id(sprite_id());
    auto hitbox_size = size();

    position = position - Vec2 { sprite_size.width / 2 - hitbox_size.width / 2, sprite_size.height / 2 - hitbox_size.height / 2 };

    sprite_manager.render_sprite_for_id_at_position(sprite_id(), position);
#if DEBUG_HITBOX == 1
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80);
    SDL_Rect rect { m_collider.x(), m_collider.y(), m_collider.w(), m_collider.h() };
    SDL_RenderDrawRect(renderer, &rect);
#endif
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
