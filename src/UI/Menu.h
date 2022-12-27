#pragma once

#include "../TextRenderer.h"
#include <SDL2/SDL.h>

namespace WizardGame {
namespace UI {

class Menu {
public:
    virtual int activate_current_selection() = 0;
    virtual void deactivate_current_selection() = 0;
    void select_next_item();
    void select_previous_item();

    virtual void render(SDL_Renderer* renderer, TextRenderer& text_renderer) const = 0;

protected:
    Menu();

    virtual int max_menu_item() const = 0;

    void render_vertical_button_list(SDL_Renderer* renderer, TextRenderer& text_renderer, const char* items[], std::size_t item_count, int y) const;

    int m_selected_menu_item;

private:
};

}
}
