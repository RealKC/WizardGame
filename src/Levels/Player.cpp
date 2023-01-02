#include "Player.h"

#include "../Utils.h"

namespace WizardGame {

Player::Player(Collider collider, Size size)
    : Entity(collider)
    , m_spawn_location { collider.x(), collider.y() }
    , m_lives(INITIAL_LIVES)
    , m_render_size(size)
    , m_iframes(0)
{
}

Player::~Player()
{
}

Bullet Player::make_bullet() const
{
    constexpr Size BULLET_SIZE = Size { 20, 20 };
    constexpr Direction DIRECTION = Direction::Up;

    return Bullet::liniar(position_for_bullet(BULLET_SIZE, DIRECTION), BULLET_SIZE, DIRECTION, 3, SpriteId::Fireball);
}

bool Player::has_iframes() const
{
    if (m_iframes == 0) {
        return false;
    }

    return true;
}

void Player::decrease_iframes()
{
    if (m_iframes) {
        // Avoid unsigned overflow
        --m_iframes;
    }
}

LostFinalLife Player::die()
{
    --m_lives;
    if (m_lives == 0) {
        return LostFinalLife::Yes;
    }

    m_iframes = 30;
    go_back_to_spawn();
    return LostFinalLife::No;
}

}
