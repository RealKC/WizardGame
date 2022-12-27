#include "Menu.h"

#include "../Game.h"
#include <algorithm>
#include <cstring>

namespace WizardGame {
namespace UI {

Menu::Menu()
    : m_selected_menu_item(0)
{
}

void Menu::select_next_item()
{
    if (m_selected_menu_item < max_menu_item()) {
        ++m_selected_menu_item;
    }
}

void Menu::select_previous_item()
{
    if (m_selected_menu_item > 0) {
        --m_selected_menu_item;
    }
}

void Menu::render_vertical_button_list(SDL_Renderer* renderer, TextRenderer& text_renderer, const char* items[], std::size_t item_count, int y) const
{
    auto* longest_item = *std::max_element(items, items + item_count, [] (auto* lhs, auto* rhs) {
        return std::strlen(lhs) < std::strlen(rhs);
    });
    auto longest_item_width = text_renderer.measure_regular_text(longest_item);

    for (std::size_t i = 0; i < item_count; ++i) {
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
