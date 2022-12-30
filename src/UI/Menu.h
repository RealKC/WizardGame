#pragma once

#include "../TextRenderer.h"
#include <SDL2/SDL.h>

namespace WizardGame {
namespace UI {

class Menu {
public:
    virtual ~Menu() {}

    virtual int activate_current_selection() = 0;
    virtual void deactivate_current_selection() = 0;
    void select_next_item();
    void select_previous_item();

    virtual void render(SDL_Renderer* renderer, TextRenderer& text_renderer) const = 0;

protected:
    Menu();

    virtual int max_menu_item() const = 0;

    int render_title(TextRenderer&, std::string const& title, int y) const;
    void render_vertical_button_list(SDL_Renderer* renderer, TextRenderer& text_renderer, char const* const* items, std::size_t item_count, int y, bool big_text = false) const;
    void render_horizontal_button_list(SDL_Renderer* renderer, TextRenderer& text_renderer, char const* items[], std::size_t item_count, Vec2 position) const;

    int m_selected_menu_item;

private:
};

}
}
