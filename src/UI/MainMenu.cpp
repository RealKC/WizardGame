#include "MainMenu.h"

#include "../Utils.h"
#include <assert.h>

namespace WizardGame {
namespace UI {

MainMenu::MainMenu()
    : m_is_in_level_selection(false)
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
        int old_selected_menu_item = m_selected_menu_item;
        m_selected_menu_item = 0;
        switch (old_selected_menu_item) {
        case 0:
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

    int y = 20;
    for (int i = 0; i < std::size(items); ++i) {
        if (m_selected_menu_item == i) {
            y += text_renderer
                     .render_regular_text_at(items[i], { 20, y }, { 0, 0, 255 })
                     .height;
        } else {
            y += text_renderer
                     .render_regular_text_at(items[i], { 20, y }, { 255, 255, 255 })
                     .height;
        }
        y += 20;
    }
}

void MainMenu::render_level_selection(SDL_Renderer* renderer, TextRenderer& text_renderer) const
{
    char const* levels[] = { "Tutorial", "1", "2", "3" };

    int x = 20;
    for (int i = 0; i < 4; ++i) {
        if (m_selected_menu_item == i) {
            x += text_renderer
                     .render_regular_text_at(levels[i], { x, 20 }, { 0, 0, 255 })
                     .width;
        } else {
            x += text_renderer
                     .render_regular_text_at(levels[i], { x, 20 }, { 255, 255, 255 })
                     .width;
        }
        x += 20;
    }
}

}
}