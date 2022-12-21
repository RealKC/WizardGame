#include "TextRenderer.h"
#include "Utils.h"

namespace WizardGame {

TextRenderer::TextRenderer()
    : m_renderer(nullptr)
    , m_regular_font(nullptr)
    , m_big_font(nullptr)
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

int TextRenderer::initialize(SDL_Renderer* renderer)
{
    static constexpr auto* FONT_NAME = "/usr/share/fonts/TTF/FiraCode-Medium.ttf";

    m_renderer = renderer;
    m_regular_font = TTF_OpenFont(FONT_NAME, 20);

    if (!m_regular_font) {
        error() << "Failed to load font '" << FONT_NAME << "' with size 20: " << TTF_GetError() << std::endl;
        return 50;
    }

    m_big_font = TTF_OpenFont(FONT_NAME, 40);

    if (!m_big_font) {
        error() << "Failed to load font '" << FONT_NAME << "' with size 40: " << TTF_GetError() << std::endl;
    }

    return RES_OK;
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
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, rendered_text);

    int text_width, text_height;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

    SDL_Rect dest { position.x, position.y, text_width, text_height };

    SDL_RenderCopy(m_renderer, texture, nullptr, &dest);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(rendered_text);

    return Size { text_width, text_height };
}

}
