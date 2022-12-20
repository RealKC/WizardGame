#include "TextRenderer.h"
#include "Utils.h"

namespace WizardGame {

TextRenderer::TextRenderer()
    : m_renderer(nullptr)
    , m_font(nullptr)
{
}

TextRenderer::~TextRenderer()
{
    if (m_font) {
        TTF_CloseFont(m_font);
    }
}

int TextRenderer::initialize(SDL_Renderer* renderer)
{
    static constexpr auto* FONT_NAME = "/usr/share/fonts/TTF/FiraCode-Medium.ttf";

    m_renderer = renderer;
    m_font = TTF_OpenFont(FONT_NAME, 20);

    if (!m_font) {
        error() << "Failed to load font '" << FONT_NAME << "': " << TTF_GetError() << std::endl;
        return 50;
    }

    return RES_OK;
}

void TextRenderer::render_text_at(std::string const& text, Vec2 position)
{
    SDL_Color color { 255, 127, 200 };
    SDL_Surface* rendered_text = TTF_RenderText_Solid(m_font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, rendered_text);

    int text_width, text_height;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

    SDL_Rect dest { position.x, position.y, text_width, text_height };

    SDL_RenderCopy(m_renderer, texture, nullptr, &dest);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(rendered_text);
}

}
