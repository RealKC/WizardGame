#pragma once

#include "Size.h"
#include "Vec2.h"
#include <SDL2/SDL_ttf.h>
#include <string>

namespace WizardGame {

class TextRenderer {
public:
    explicit TextRenderer(SDL_Renderer*);
    ~TextRenderer();

    int big_text_height() const { return 40; }

    // These return the width of the rendered text
    int measure_regular_text(std::string const& text);
    int measure_big_text(std::string const& text);

    // These return the size of the rendered text, to be used in menus and such
    Size render_regular_text_at(std::string const& text, Vec2 position, SDL_Color color);
    Size render_big_text_at(std::string const& text, Vec2 position, SDL_Color color);

    Size render_wrapped_big_text_at(std::string const& text, Vec2 position, SDL_Color color);

    // We need to call this before TTF_Quit or SDL_Quit, so we can't have the destructor
    // handle it for us
    void close_fonts();

private:
    int measure_text(TTF_Font*, std::string const& text);
    Size render_text_at(TTF_Font*, std::string const& text, Vec2 position, SDL_Color color);
    Size render_wrapped_text_at(TTF_Font*, std::string const& text, Vec2 position, SDL_Color color);

    Size render_surface(SDL_Surface*, Vec2 position, char const* error_category);

    SDL_Renderer* m_renderer;
    TTF_Font* m_regular_font;
    TTF_Font* m_big_font;
};

}
