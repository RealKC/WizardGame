#include "Enemy.h"

#include "Utils.h"

namespace WizardGame {

Enemy::Enemy(Collider collider, Vec2 target_position)
    : Entity(collider)
    , m_target_position(target_position)
    , m_cooldown(0)
{
}

std::unique_ptr<Enemy> make_enemy(Vec2 starting_position, Vec2 target_position, Enemy::Type type)
{
    switch (type) {
    case Enemy::Type::Basic:
        return std::unique_ptr<Enemy>(new BasicEnemy(Collider(starting_position.x, starting_position.y, 50, 50), target_position));
    case Enemy::Type::Adrian1:
        return std::unique_ptr<Enemy>(new AdrianEnemy(Collider(0, 0, 0, 0), target_position, 1));
    case Enemy::Type::Adrian2:
        return std::unique_ptr<Enemy>(new AdrianEnemy(Collider(0, 0, 0, 0), target_position, 2));
    default:
        error() << "Unknown enemy type: " << (int)type << "\n";
    }
}

void Enemy::move_to_target_position()
{
    if (m_target_position == position())
        return;

    if (m_cooldown == 0) {
        m_cooldown = 3;
        move_by(0, -1);
        return;
    }

    --m_cooldown;
}

void BasicEnemy::tick(std::vector<Bullet>& bullets, uint32_t current_time)
{
    move_to_target_position();

    bullets.push_back(Bullet::radial(position_for_bullet({ 40, 40 }, Direction::Left), { 40, 40 }, 1, M_PI));
}

void BasicEnemy::render(SDL_Renderer* renderer)
{
    SDL_Rect rect { position().x, position().y, size().width, size().height };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

void AdrianEnemy::tick(std::vector<Bullet>& bullets, uint32_t current_time)
{
    move_to_target_position();
}

}
