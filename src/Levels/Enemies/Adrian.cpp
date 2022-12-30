#include "Adrian.h"

namespace WizardGame {
namespace Enemies {

bool Adrian::has_pending_dialog() const
{
    return !m_dialog.empty() && health_percentage() == 0.5;
}

Enemy::Died Adrian::hit()
{
    if (m_health - 10 == 0) {
        return Died::Yes;
    }

    m_health -= 10;
    return Died::No;
}

void Adrian::tick(std::vector<Bullet>& bullets, uint32_t current_time)
{
    move_to_target_position();
    fire_attacks(current_time, bullets);
}

}
}
