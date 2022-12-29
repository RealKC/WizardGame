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
        return ActivationResult::Restart;
    case 2:
        return ActivationResult::QuitToMenu;
    case 3:
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

    auto y = render_title(text_renderer, "-- Paused --", Game::WINDOW_HEIGHT / 3);

    y += 35;

    char const* items[] = { "Continue", "Restart", "Quit to menu", "Quit to desktop" };

    render_vertical_button_list(renderer, text_renderer, items, std::size(items), y);
}

}
}
