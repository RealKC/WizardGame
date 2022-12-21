#pragma once

#include "Size.h"
#include "Vec2.h"
#include <SDL2/SDL_ttf.h>
#include <string>

namespace WizardGame {

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    int initialize(SDL_Renderer*);

    // These return the size of the rendered text, to be used in menus and such
    Size render_regular_text_at(std::string const& text, Vec2 position, SDL_Color color);
    Size render_big_text_at(std::string const& text, Vec2 position, SDL_Color color);

    // We need to call this before TTF_Quit or SDL_Quit, so we can't have the destructor
    // handle it for us
    void close_fonts();
private:
    Size render_text_at(TTF_Font* font, std::string const& text, Vec2 position, SDL_Color color);
    SDL_Renderer* m_renderer;
    TTF_Font* m_regular_font;
    TTF_Font* m_big_font;
};

}
