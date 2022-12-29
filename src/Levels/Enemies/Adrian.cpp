#include "Adrian.h"

namespace WizardGame {
namespace Enemies {

Enemy::Died Adrian::hit()
{
    if (m_health - 10 == 0) {
        return Died::Yes;
    }

    m_health -= 10;
    return Died::No;
}

void Adrian::tick(std::vector<Bullet>& bullets, uint32_t current_time)
{
    move_to_target_position();
    fire_attacks(current_time, bullets);
}

void Adrian::render(SDL_Renderer* renderer, SpriteManager&)
{
    SDL_Rect rect { position().x, position().y, size().width, size().height };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

}
}
