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
    constexpr Direction DIRECTION = Direction::Up;

    return Bullet::liniar(position_for_bullet(BULLET_SIZE, DIRECTION), BULLET_SIZE, DIRECTION);
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_Rect player_rect { position().x, position().y, size().width, size().height };
    SDL_RenderFillRect(renderer, &player_rect);
}

}
