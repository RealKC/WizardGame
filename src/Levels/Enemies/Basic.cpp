#include "Basic.h"

#include <stdlib.h>

namespace WizardGame {
namespace Enemies {

Basic::Basic(Collider collider, Vec2 target_position, std::vector<Attack> attacks)
    : Enemy(collider, target_position, attacks)
{
    // Pick a random sprite for basic enemies
    // This relies on the three basic enemy sprites having consecutive values.
    auto sprite = static_cast<int>(SpriteId::Basic1) + rand() % 3;
    m_sprite_id = static_cast<SpriteId>(sprite);
}

void Basic::tick(std::vector<Bullet>& bullets, uint32_t current_time)
{
    move_to_target_position();
    fire_attacks(current_time, bullets);

    // bullets.push_back(Bullet::radial(position_for_bullet({ 40, 40 }, Direction::Left), { 40, 40 }, 1, M_PI));
}

void Basic::render(SDL_Renderer* renderer, SpriteManager&)
{
    SDL_Rect rect { position().x, position().y, size().width, size().height };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

}
}
