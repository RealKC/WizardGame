#include "AbstractLevelMenu.h"

#include "../Game.h"
#include <algorithm>
#include <assert.h>

namespace WizardGame {
namespace UI {

AbstractLevelMenu::AbstractLevelMenu(std::vector<int> custom_activation_results, std::string title, std::vector<char const*> items)
    : m_activation_results(custom_activation_results)
    , m_title(title)
    , m_items(items)
{
}

int AbstractLevelMenu::activate_current_selection()
{
    if (m_selected_menu_item < m_activation_results.size()) {
        return m_activation_results[m_selected_menu_item];
    }

    switch (m_selected_menu_item + selection_offset()) {
    case 2:
        return ActivationResult::QuitToMenu;
    case 3:
        return ActivationResult::QuitToDesktop;
    }

    assert(false);
}

void AbstractLevelMenu::deactivate_current_selection()
{
    // nothing to do
}

void AbstractLevelMenu::render(SDL_Renderer* renderer, TextRenderer& text_renderer) const
{
    // Dim the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect rect { 0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT };
    SDL_RenderFillRect(renderer, &rect);

    auto y = render_title(text_renderer, m_title, Game::WINDOW_HEIGHT / 3);

    y += 35;

    render_vertical_button_list(renderer, text_renderer, m_items.data(), m_items.size(), y);
}

}
}
