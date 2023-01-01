#pragma once

#include <cstdint>

namespace WizardGame {
namespace Save {
enum class Level {
    None,
    Tutorial,
    One,
    Two,
    Three,
};

std::int64_t get_high_score_for(Level);
void set_high_score_for(Level, std::int64_t high_score);


Level get_last_beat_level();
void set_last_beat_level(Level);

}
}
