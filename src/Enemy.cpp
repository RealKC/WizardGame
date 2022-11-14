#include "Enemy.h"

#include "Utils.h"

namespace WizardGame {

std::unique_ptr<Enemy> make_enemy(Vec2 starting_position, Enemy::Type type)
{
    switch (type) {
    case Enemy::Type::Basic:
        return std::unique_ptr<Enemy>(new BasicEnemy(Collider(0, 0, 0, 0)));
    case Enemy::Type::Adrian1:
        return std::unique_ptr<Enemy>(new AdrianEnemy(Collider(0, 0, 0, 0), 1));
    case Enemy::Type::Adrian2:
        return std::unique_ptr<Enemy>(new AdrianEnemy(Collider(0, 0, 0, 0), 2));
    default:
        error() << "Unknown enemy type: " << (int)type << "\n";
    }
}

void BasicEnemy::tick(uint32_t current_time) { }

void AdrianEnemy::tick(uint32_t current_time) { }

}
