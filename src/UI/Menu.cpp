#include "Menu.h"

#include "../Game.h"
#include "../Utils.h"
#include <algorithm>
#include <cstring>

namespace WizardGame {
namespace UI {

Menu::Menu(SDL_Color normal, SDL_Color selected)
    : m_selected_menu_item(0)
    , m_selected_text_color(selected)
    , m_normal_text_color(normal)
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

int Menu::render_title(TextRenderer& text_renderer, std::string const& title, int y) const
{
    auto width = text_renderer.measure_big_text(title);

    y += text_renderer
             .render_big_text_at(title, { Game::WINDOW_WIDTH / 2 - width / 2, y }, { 140, 10, 100 })
             .height;

    return y;
}

void Menu::render_vertical_button_list(SDL_Renderer* renderer, TextRenderer& text_renderer, char const* const* items, std::size_t item_count, int y, bool big_text) const
{
    auto* longest_item = *std::max_element(items, items + item_count, [](auto* lhs, auto* rhs) {
        return std::strlen(lhs) < std::strlen(rhs);
    });
    auto longest_item_width = big_text
        ? text_renderer.measure_big_text(longest_item)
        : text_renderer.measure_regular_text(longest_item);

    for (std::size_t i = 0; i < item_count; ++i) {
        auto width = big_text
            ? text_renderer.measure_big_text(items[i])
            : text_renderer.measure_regular_text(items[i]);
        auto height = big_text
            ? text_renderer.big_text_height()
            : text_renderer.regular_text_height();
        auto x = Game::WINDOW_WIDTH / 2 - width / 2;
        if (m_selected_menu_item == i) {
            SDL_Rect button_rect { Game::WINDOW_WIDTH / 2 - longest_item_width / 2 - 5, y - 5, longest_item_width + 10, height + 15 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
            SDL_RenderFillRect(renderer, &button_rect);

            if (big_text) {
                y += text_renderer
                         .render_big_text_at(items[i], { x, y }, m_selected_text_color)
                         .height;

            } else {
                y += text_renderer
                         .render_regular_text_at(items[i], { x, y }, m_selected_text_color)
                         .height;
            }
        } else {
            if (big_text) {
                y += text_renderer
                         .render_big_text_at(items[i], { x, y }, m_normal_text_color)
                         .height;
            } else {
                y += text_renderer
                         .render_regular_text_at(items[i], { x, y }, m_normal_text_color)
                         .height;
            }
        }
        y += 20;
    }
}

void Menu::render_horizontal_button_list(SDL_Renderer* renderer, TextRenderer& text_renderer, char const* items[], std::size_t item_count, Vec2 position) const
{
    auto x = position.x;
    for (std::size_t i = 0; i < item_count; ++i) {
        if (m_selected_menu_item == i) {
            auto width = text_renderer.measure_big_text(items[i]);
            SDL_Rect button_rect { x - 5, position.y - 5, width + 5, text_renderer.big_text_height() + 15 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
            SDL_RenderFillRect(renderer, &button_rect);

            x += text_renderer
                     .render_big_text_at(items[i], { x, position.y }, m_selected_text_color)
                     .width;
        } else {
            x += text_renderer
                     .render_big_text_at(items[i], { x, position.y }, m_normal_text_color)
                     .width;
        }
        x += 20;
    }
}

}
}
