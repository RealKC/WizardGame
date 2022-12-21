#pragma once

#include "TextRenderer.h"
#include <SDL2/SDL.h>

namespace WizardGame {

class Menu {
public:
    enum Direction {
        Up,
        Down,
    };

    void change_selection(Direction);
    void render(SDL_Renderer* renderer, TextRenderer const& text_renderer);
private:
    int m_selected_menu_item;
};

}
