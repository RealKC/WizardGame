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

    char const* backgrounds[] = {
        "resources/backgrounds/menu.jpg",
        "resources/backgrounds/tutorial.jpg",
        "resources/backgrounds/1.jpg",
        "resources/backgrounds/2.jpg",
        "resources/backgrounds/3.jpg",
    };

    for (std::size_t i = 0; i < BACKGROUND_COUNT; ++i) {
        auto* background = IMG_Load(backgrounds[i]);

        if (!background) {
            throw SDLObjectError("SpriteManager/background", FailureTo::Load, backgrounds[i]);
        }

        m_backgrounds[i] = SDL_CreateTextureFromSurface(renderer, background);

        if (!m_backgrounds[i]) {
            throw SDLObjectError("SpriteManager/background", FailureTo::Create, "background texture");
        }
    }

    char const* portraits[] = {
        "resources/sprites/portrait-adrian.jpeg",
        "resources/sprites/portrait-matei.jpeg",
        "resources/sprites/portrait-mihai.jpeg",
        "resources/sprites/portrait-mircea.jpeg",
    };

    for (std::size_t i = 0; i < PORTRAIT_COUNT; ++i) {
        auto* portrait = IMG_Load(portraits[i]);

        if (!portrait) {
            throw SDLObjectError("SpriteManager/portrait", FailureTo::Load, portraits[i]);
        }

        m_portraits[i] = SDL_CreateTextureFromSurface(renderer, portrait);

        if (!m_portraits[i]) {
            throw SDLObjectError("SpriteManager/portrait", FailureTo::Create, "portrait texture");
        }
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!m_texture) {
        error() << "Failed to create texture from surface " << SDL_GetError() << std::endl;
        throw SDLObjectError("SpriteManager/texture", FailureTo::Create, "image texture");
    }

    info() << "Succesfully loaded 'sprites'!\n";
}

SpriteManager::~SpriteManager()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }

    for (std::size_t i = 0; i < BACKGROUND_COUNT; ++i) {
        if (m_backgrounds[i]) {
            SDL_DestroyTexture(m_backgrounds[i]);
        }
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

void SpriteManager::render_portrait_at(PortraitId portrait_id, Vec2 position) const
{
    auto* portrait = m_portraits[static_cast<std::size_t>(portrait_id)];
    auto portrait_size = size_for_portrait_id(portrait_id);
    SDL_Rect destination_rect { position.x, position.y, portrait_size.width, portrait_size.height };
    SDL_RenderCopy(m_renderer, portrait, nullptr, &destination_rect);
}

void SpriteManager::render_background(BackgroundId background_id) const
{
     SDL_RenderCopy(m_renderer, m_backgrounds[static_cast<std::size_t>(background_id)], nullptr, nullptr);
}

SDL_Rect SpriteManager::sprite_id_to_source_rect(SpriteId)
{
    // FIXME: Actually implement this!
    return SDL_Rect();
}

}
