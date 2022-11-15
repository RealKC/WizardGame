#include "Adrian.h"

namespace WizardGame {
namespace Enemies {

void Adrian::tick(std::vector<Bullet>& bullets, uint32_t current_time)
{
    move_to_target_position();
}

}
}
