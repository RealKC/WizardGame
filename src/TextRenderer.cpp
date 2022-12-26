#include "TextRenderer.h"

#include "Exceptions/TTFObjectError.h"
#include "Game.h"
#include "Utils.h"

namespace WizardGame {

static constexpr auto* FONT_NAME = "/usr/share/fonts/TTF/FiraCode-Medium.ttf";

static TTF_Font* open_font(const char* font_name, int ptsize) {
    auto* font = TTF_OpenFont(font_name, ptsize);
    if (!font) {
        throw TTFObjectError("TextRenderer", font_name, ptsize);
    }
    return font;
}

TextRenderer::TextRenderer(SDL_Renderer* renderer)
    : m_renderer(renderer)
    , m_regular_font(open_font(FONT_NAME, 20))
    , m_big_font(open_font(FONT_NAME, 40))
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::close_fonts()
{
    if (m_regular_font) {
        TTF_CloseFont(m_regular_font);
    }

    if (m_big_font) {
        TTF_CloseFont(m_big_font);
    }
}

Size TextRenderer::render_regular_text_at(std::string const& text, Vec2 position, SDL_Color color)
{
    return render_text_at(m_regular_font, text, position, color);
}

Size TextRenderer::render_big_text_at(std::string const& text, Vec2 position, SDL_Color color)
{
    return render_text_at(m_big_font, text, position, color);
}

Size TextRenderer::render_text_at(TTF_Font* font, std::string const& text, Vec2 position, SDL_Color color)
{
    SDL_Surface* rendered_text = TTF_RenderText_Solid(font, text.c_str(), color);

    if (!rendered_text) {
        throw TTFObjectError("TextRenderer/render_text", "Failed to render text");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, rendered_text);

    if (!texture) {
        throw SDLObjectError("TextRenderer/render_text", FailureTo::Create, "texture from rendered text");
    }

    int text_width = 0, text_height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

    SDL_Rect dest { position.x, position.y, text_width, text_height };

    SDL_RenderCopy(m_renderer, texture, nullptr, &dest);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(rendered_text);

    return Size { text_width, text_height };
}

int TextRenderer::measure_big_text(std::string const& text)
{
    return measure_text(m_big_font, text);
}

int TextRenderer::measure_regular_text(std::string const& text)
{
    return measure_text(m_regular_font, text);
}

int TextRenderer::measure_text(TTF_Font* font, std::string const& text)
{
    int count, width;
    TTF_MeasureUTF8(font, text.c_str(), Game::WINDOW_WIDTH, &width, &count);
    return width;
}

}
