#include "Enemy.h"

#include "Utils.h"

namespace WizardGame {

std::unique_ptr<Enemy> Enemy::spawn(Vec2 starting_position, Type type)
{
    switch (type) {
    case Type::Basic:
        return std::unique_ptr<Enemy>(new BasicEnemy(Collider(0, 0, 0, 0)));
    case Type::Adrian1:
        return std::unique_ptr<Enemy>(new AdrianEnemy(Collider(0, 0, 0, 0), 1));
    case Type::Adrian2:
        return std::unique_ptr<Enemy>(new AdrianEnemy(Collider(0, 0, 0, 0), 2));
    default:
        error() << "Unknown enemy type: " << (int)type << "\n";
    }
}

void BasicEnemy::tick(uint32_t current_time) { }

void AdrianEnemy::tick(uint32_t current_time) { }

}
