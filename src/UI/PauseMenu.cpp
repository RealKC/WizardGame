#include "PauseMenu.h"

#include "../Game.h"
#include <algorithm>
#include <assert.h>

namespace WizardGame {
namespace UI {

int PauseMenu::activate_current_selection()
{
    switch (m_selected_menu_item) {
    case 0:
        return ActivationResult::Continue;
    case 1:
        return ActivationResult::QuitToMenu;
    case 2:
        return ActivationResult::QuitToDesktop;
    }

    assert(false);
}

void PauseMenu::deactivate_current_selection()
{
    // nothing to do
}

void PauseMenu::render(SDL_Renderer* renderer, TextRenderer& text_renderer) const
{
    // Dim the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect rect { 0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT };
    SDL_RenderFillRect(renderer, &rect);

    int y = Game::WINDOW_HEIGHT / 3;

    char const* pause = "-- Paused --";
    auto width = text_renderer.measure_big_text(pause);

    y += text_renderer
             .render_big_text_at(pause, { Game::WINDOW_WIDTH / 2 - width / 2, y }, { 140, 10, 100 })
             .height;

    y += 35;

    char const* items[] = { "Continue", "Quit to menu", "Quit to desktop" };

    render_vertical_button_list(renderer, text_renderer, items, std::size(items), y);
}

}
}
