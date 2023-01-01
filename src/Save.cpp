#include "Save.h"

#include <assert.h>
#include <fstream>
#include <string>
#include "Utils.h"

namespace WizardGame {
namespace Save {

static std::string get_score_file_for(Level level)
{
    switch (level) {
    case Level::None:
        assert(false);
    case Level::Tutorial:
        return "saves/tutorial.uwu";
    case Level::One:
        return "saves/1.uwu";
    case Level::Two:
        return "saves/2.uwu";
    case Level::Three:
        return "saves/3.uwu";
    }

    assert(false && "Invalid level");
}

std::int64_t get_high_score_for(Level level)
{
    std::ifstream save(get_score_file_for(level));
    std::int64_t high_score = 0;
    save >> high_score;
    return high_score;
}

void set_high_score_for(Level level, std::int64_t high_score)
{
    std::ofstream save(get_score_file_for(level), std::ofstream::trunc);

    save << high_score;
}

Level get_last_beat_level()
{
    std::ifstream file("saves/level.owo");
    std::string level;
    file >> level;

    if (level == "tutorial") {
        return Level::Tutorial;
    }

    if (level == "1") {
        return Level::One;
    }

    if (level == "2") {
        return Level::Two;
    }

    if (level == "3") {
        return Level::Three;
    }

    return Level::None;
}

void set_last_beat_level(Level level) {
    std::ofstream file("saves/level.owo");
    switch (level) {
    case Level::Tutorial:
        file << "tutorial";
        break;
    case Level::One:
        file << "1";
        break;
    case Level::Two:
        file << "2";
        break;
    case Level::Three:
        file << "3";
        break;
    default:
        break;
    }
}

}
}
