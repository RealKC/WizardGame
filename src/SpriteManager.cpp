#include "SpriteManager.h"

#include "Exceptions/SDLObjectError.h"
#include "Utils.h"
#include <SDL2/SDL_image.h>

namespace WizardGame {

SpriteManager::SpriteManager(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
    SDL_Surface* surface = IMG_Load("resources/sprites/spritesheet.jpg");

    if (!surface) {
        throw SDLObjectError("SpriteManager/spritesheet", FailureTo::Load, "image surface");
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!m_texture) {
        error() << "Failed to create texture from surface " << SDL_GetError() << std::endl;
        throw SDLObjectError("SpriteManager/texture", FailureTo::Create, "image texture");
    }
}

SpriteManager::~SpriteManager()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }

    // We MUST NOT destroy the renderer here as we do not own it!!
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
