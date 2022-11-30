#pragma once

#include "../Enemy.h"
#include "Adrian.h"
#include "Attack.h"
#include "Basic.h"
#include <string>
#include <vector>

namespace WizardGame {
namespace Enemies {

class Manager {
public:
    Manager(std::string const& path);

    std::unique_ptr<Enemy> adrian(Collider, Vec2 position, int phase) const;
    std::unique_ptr<Enemy> basic(Collider, Vec2 position) const;

private:
    bool m_is_boss_level;
    std::vector<Attack> m_basic_enemies_attacks;
};

Attack parse_attack();

}
}
