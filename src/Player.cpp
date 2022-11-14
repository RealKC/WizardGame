#include "Player.h"

namespace WizardGame {

Player::Player(Collider collider)
    : Entity(collider)
{
}

Player::~Player()
{
}

Bullet Player::make_bullet() const
{
    constexpr Size BULLET_SIZE = Size { 20, 20 };
    auto bullet_coords = Vec2 { position().x + size().width / 2 - BULLET_SIZE.width / 2, position().y - BULLET_SIZE.width };

    return Bullet::liniar(bullet_coords, BULLET_SIZE, Direction::Up);
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_Rect player_rect { position().x, position().y, size().width, size().height };
    SDL_RenderFillRect(renderer, &player_rect);
}

}
