#include "PauseMenu.h"

#include "../Game.h"
#include <algorithm>
#include <assert.h>
#include <cstring>

namespace WizardGame {
namespace UI {

static char const* find_longest_item(char const* items[], std::size_t count) {
    std::size_t idx = 0;
    std::size_t max_length = 0;
    for (std::size_t i = 0; i < count; ++i) {
        auto curr_len = std::strlen(items[i]);
    }
}

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

    auto* longest_item = *std::max_element(items, items + std::size(items), [] (auto* lhs, auto* rhs) {
        return std::strlen(lhs) < std::strlen(rhs);
    });
    auto longest_item_width = text_renderer.measure_regular_text(longest_item);

    for (int i = 0; i < std::size(items); ++i) {
        auto width = text_renderer.measure_regular_text(items[i]);
        auto x = Game::WINDOW_WIDTH / 2 - width / 2;
        if (m_selected_menu_item == i) {
            SDL_Rect button_rect {Game::WINDOW_WIDTH / 2 - longest_item_width / 2 - 5, y - 5, longest_item_width + 10, 35};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_RenderFillRect(renderer, &button_rect);

            y += text_renderer
                     .render_regular_text_at(items[i], { x, y }, { 0, 0, 255 })
                     .height;

        } else {
            y += text_renderer
                     .render_regular_text_at(items[i], { x, y }, { 255, 255, 255 })
                     .height;
        }
        y += 20;
    }
}

}
}
