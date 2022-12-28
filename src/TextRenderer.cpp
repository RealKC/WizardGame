#include "TextRenderer.h"

#include "Exceptions/TTFObjectError.h"
#include "Game.h"
#include "Utils.h"
#include <sstream>

namespace WizardGame {

static constexpr auto* FONT_NAME = "/usr/share/fonts/TTF/FiraCode-Medium.ttf";

static TTF_Font* open_font(char const* font_name, int ptsize)
{
    auto* font = TTF_OpenFont(font_name, ptsize);
    if (!font) {
        throw TTFObjectError("TextRenderer", font_name, ptsize);
    }
    return font;
}

TextRenderer::TextRenderer(SDL_Renderer* renderer)
    : m_renderer(renderer)
    , m_regular_font(open_font(FONT_NAME, 20))
    , m_big_font(open_font(FONT_NAME, big_text_height()))
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
    SDL_Surface* rendered_text = TTF_RenderUTF8_Blended(font, text.c_str(), color);

    if (!rendered_text) {
        throw TTFObjectError("TextRenderer/render_text", "Failed to render text");
    }

    return render_surface(rendered_text, position, "TextRenderer/render_text+surface");
}

Size TextRenderer::render_wrapped_big_text_at(std::string const& text, Vec2 position, SDL_Color color)
{
    return render_wrapped_text_at(m_big_font, text, position, color);
}

Size TextRenderer::render_wrapped_text_at(TTF_Font* font, std::string const& text, Vec2 position, SDL_Color color)
{
    auto longest_substring = [](std::string const& text) {
        std::stringstream stream(text);
        std::string longest, curr;

        while (stream >> curr) {
            if (curr.length() > longest.length()) {
                longest = curr;
            }
        }

        return longest;
    };

    auto wrap_length = measure_text(font, longest_substring(text));

    SDL_Surface* rendered_text = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrap_length);

    if (!rendered_text) {
        throw TTFObjectError("TextRenderer/render_wrapped_text", "Failed to render text");
    }

    return render_surface(rendered_text, position, "TextRenderer/render_wrapped_text+surface");
}

Size TextRenderer::render_surface(SDL_Surface* surface, Vec2 position, char const* error_category)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);

    if (!texture) {
        throw SDLObjectError(error_category, FailureTo::Create, "texture from surface");
    }

    int text_width = 0, text_height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

    SDL_Rect dest { position.x, position.y, text_width, text_height };

    SDL_RenderCopy(m_renderer, texture, nullptr, &dest);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

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
