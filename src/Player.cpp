#include "Player.h"

namespace WizardGame {

Player::Player(Collider collider)
    : Entity(collider)
{
}

Player::~Player()
{
}

Bullet Player::shoot() const
{
    constexpr Size BULLET_SIZE = Size { 40, 40 };
    auto bullet_coords = position() - Vec2 { 0, BULLET_SIZE.width };

    return Bullet::liniar(bullet_coords, BULLET_SIZE, Direction::Up);
}

}
