#include "Basic.h"

namespace WizardGame {
namespace Enemies {

void Basic::tick(std::vector<Bullet>& bullets, uint32_t current_time)
{
    move_to_target_position();

    bullets.push_back(Bullet::radial(position_for_bullet({ 40, 40 }, Direction::Left), { 40, 40 }, 1, M_PI));
}

void Basic::render(SDL_Renderer* renderer)
{
    SDL_Rect rect { position().x, position().y, size().width, size().height };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

}
}
