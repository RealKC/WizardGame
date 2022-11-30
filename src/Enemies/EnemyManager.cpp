#include "EnemyManager.h"

#include "../Utils.h"
#include <cctype>
#include <fstream>
#include <unistd.h>

namespace WizardGame {
namespace Enemies {

Attack::Type parse_attack(char ch)
{
    switch (ch) {
    case 'c':
        return Attack::Type::Circle;
    case 'l':
        return Attack::Type::Line;
    case 't':
        return Attack::Type::ThreeAtOnce;
    }
}

Manager::Manager(std::string const& path)
{
    std::ifstream attacks(path);

    error() << get_current_dir_name() << std::endl;

    char ch;
    int cooldown;
    while (attacks >> ch >> cooldown) {
        error() << "parse...\n";
        m_basic_enemies_attacks.push_back(Attack { parse_attack(ch), cooldown, 0 });
    }
}
std::unique_ptr<Enemy> Manager::basic(Collider collider, Vec2 position) const
{
    auto attack = m_basic_enemies_attacks[rand() % m_basic_enemies_attacks.size()];
    return std::make_unique<Basic>(collider, position, std::vector { attack });
}
std::unique_ptr<Enemy> Manager::adrian(Collider collider, Vec2 position, int phase) const
{
    return std::make_unique<Adrian>(collider, position);
}

}
}
