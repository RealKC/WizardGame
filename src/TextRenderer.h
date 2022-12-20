#pragma once

#include "Vec2.h"
#include <SDL2/SDL_ttf.h>
#include <string>

namespace WizardGame {

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    int initialize(SDL_Renderer*);

    void render_text_at(std::string const& text, Vec2 position);

private:
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
};

}
