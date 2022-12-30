#include "MainMenu.h"

#include "../Game.h"
#include "../Utils.h"
#include <assert.h>

namespace WizardGame {
namespace UI {

MainMenu::MainMenu()
    : Menu({ 0x00, 0x42, 0x25 }, { 0, 0, 0xff })
    , m_is_in_level_selection(false)
{
}

int MainMenu::max_menu_item() const
{
    return m_is_in_level_selection ? 3 : 1;
}

int MainMenu::activate_current_selection()
{
    info() << "Current selection is: " << m_selected_menu_item << std::endl;
    if (m_is_in_level_selection) {
        info() << "Selected a level: " << m_selected_menu_item << std::endl;
        switch (m_selected_menu_item) {
        case 0:
            return ActivationResult::Tutorial;
        case 1:
            return ActivationResult::Level1;
        case 2:
            return ActivationResult::Level2;
        case 3:
            return ActivationResult::Level3;
        default:
            assert(false);
        }
    } else {
        m_is_in_level_selection = (m_selected_menu_item == 0);
        switch (m_selected_menu_item) {
        case 0:
            m_selected_menu_item = 0;
            return ActivationResult::None;
        case 1:
            return ActivationResult::Quit;
        default:
            assert(false);
        }
    }
}

void MainMenu::deactivate_current_selection()
{
    m_selected_menu_item = 0;
    m_is_in_level_selection = false;
}

void MainMenu::render(SDL_Renderer* renderer, TextRenderer& text_renderer) const
{

    if (m_is_in_level_selection) {
        render_level_selection(renderer, text_renderer);
    } else {
        render_main_menu(renderer, text_renderer);
    }
}

void MainMenu::render_main_menu(SDL_Renderer* renderer, TextRenderer& text_renderer) const
{
    char const* items[] = { "Play", "Exit" };

    render_vertical_button_list(renderer, text_renderer, items, std::size(items), Game::WINDOW_HEIGHT / 3, true);
}

void MainMenu::render_level_selection(SDL_Renderer* renderer, TextRenderer& text_renderer) const
{
    char const* levels[] = { "Tutorial", "1", "2", "3" };

    char const* choose_a_level = "Choose a level: ";
    auto y = Game::WINDOW_HEIGHT / 2 - 200;
    auto width = text_renderer.measure_big_text(choose_a_level);

    y += text_renderer.render_big_text_at(choose_a_level, { Game::WINDOW_WIDTH / 2 - width / 2 + 25, y }, { 0, 0, 0 })
             .height;

    y += 30;

    render_horizontal_button_list(renderer, text_renderer, levels, std::size(levels), { static_cast<int>(Game::WINDOW_WIDTH / 2.75), y });
}

}
}
