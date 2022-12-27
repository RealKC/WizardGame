#include "TutorialLevel.h"

namespace WizardGame {

TutorialLevel::TutorialLevel(uint32_t level_event)
    : Level(level_event)
    , m_tutorial_point(0)
{
}

void TutorialLevel::run_frame_impl(uint32_t current_time)
{
}

void TutorialLevel::render_impl(SDL_Renderer* renderer, TextRenderer& text_renderer, SpriteManager&)
{
    char const* tips[] = { "Move with arrows", "Shoot with x", "KILL THEM!!!" };

    static_assert(std::size(tips) == DIALOGUE_COUNT);

    text_renderer.render_big_text_at(tips[m_tutorial_point], { 40, 40 }, { 0, 0, 0 });
}

void TutorialLevel::dismiss_dialogue_if_any()
{
    if (m_tutorial_point + 1 < DIALOGUE_COUNT) {
        m_tutorial_point++;
    }
}

void TutorialLevel::kill_player()
{
}

void TutorialLevel::restart_level()
{
    Level::restart_level();
    m_tutorial_point = 0;
}

}
