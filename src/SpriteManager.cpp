#include "SpriteManager.h"

#include "Utils.h"
#include <SDL2/SDL_image.h>

namespace WizardGame {

SpriteManager::SpriteManager()
    : m_renderer(nullptr)
{
}

SpriteManager::~SpriteManager()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }

    // We MUST NOT destroy the renderer here as we do not own it!!
}

int SpriteManager::initialize(SDL_Renderer* renderer)
{
    m_renderer = renderer;
    SDL_Surface* surface = IMG_Load("resources/sprites/spritesheet.jpg");

    if (surface) {
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!m_texture) {
            error() << "Failed to create texture from surface " << SDL_GetError() << std::endl;
            return 6;
        }
    } else {
        error() << "Failed to load surface from image: " << SDL_GetError() << std::endl;
        return 5;
    }

    return RES_OK;
}

void SpriteManager::render_sprite_for_id_at_position(SpriteId, Vec2)
{
    // FIXME: Actually implement this!
    // FIXME: auto source_rect = sprite_id_to_source_rect(id);
    // FIXME: SDL_Rect destination_rect { pos.x, pos.y, source_rect.w, source_rect.h };
    // FIXME: SDL_RenderCopy(m_renderer, m_texture, &source_rect, &destination_rect);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
}
SDL_Rect SpriteManager::sprite_id_to_source_rect(SpriteId)
{
    // FIXME: Actually implement this!
    return SDL_Rect();
}

}
