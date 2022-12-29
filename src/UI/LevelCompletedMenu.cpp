#include "LevelCompletedMenu.h"

namespace WizardGame {
namespace UI {

std::vector<int> LevelCompletedMenu::custom_activation_results(bool has_next_level)
{
    if (has_next_level) {
        return { ActivationResult::NextLevel };
    } else {
        return {};
    }
}

std::string LevelCompletedMenu::title(bool has_next_level)
{
    if (has_next_level) {
        return "-- You won the level --";
    } else {
        return "-- You won the game! --";
    }
}

std::vector<char const*> LevelCompletedMenu::items(bool has_next_level)
{
    if (has_next_level) {
        return { "Next level", "Quit to menu", "Quit to desktop" };
    } else {
        return { "Quit to menu", "Quit to desktop" };
    }
};

}
}
