#include "SpriteManager.h"

#include "Exceptions/SDLObjectError.h"
#include "Utils.h"
#include <SDL2/SDL_image.h>
#include <assert.h>

namespace WizardGame {

SpriteManager::SpriteManager(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
    SDL_Surface* surface = IMG_Load("resources/sprites/spritesheet.png");

    if (!surface) {
        throw SDLObjectError("SpriteManager/spritesheet", FailureTo::Load, "image surface");
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!m_texture) {
        error() << "Failed to create texture from surface " << SDL_GetError() << std::endl;
        throw SDLObjectError("SpriteManager/texture", FailureTo::Create, "image texture");
    }

    surface = IMG_Load("resources/sprites/frame.png");

    if (!surface) {
        throw SDLObjectError("SpriteManager/frame", FailureTo::Load, "frame surface");
    }

    m_frame = SDL_CreateTextureFromSurface(renderer, surface);

    if (!m_frame) {
        throw SDLObjectError("SpriteManager/frame", FailureTo::Create, "frame texture");
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

    for (std::size_t i = 0; i < PORTRAIT_COUNT; ++i) {
        if (m_portraits[i]) {
            SDL_DestroyTexture(m_portraits[i]);
        }
    }

    // We MUST NOT destroy the renderer here as we do not own it!!
}

Size SpriteManager::render_sprite_for_id_at_position(SpriteId sprite_id, Vec2 position, int scale)
{
    auto source_rect = sprite_id_to_source_rect(sprite_id);
    SDL_Rect destination_rect { position.x, position.y, source_rect.w * scale, source_rect.h * scale };
    SDL_RenderCopy(m_renderer, m_texture, &source_rect, &destination_rect);

    return Size { destination_rect.w, destination_rect.h };
}

void SpriteManager::render_frame_at(Vec2 position) const
{
    int width, height;
    SDL_QueryTexture(m_frame, nullptr, nullptr, &width, &height);
    SDL_Rect rect { position.x, position.y, width, height };
    SDL_RenderCopy(m_renderer, m_frame, nullptr, &rect);
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

SDL_Rect SpriteManager::sprite_id_to_source_rect(SpriteId sprite_id)
{
    switch (sprite_id) {
    case SpriteId::Player:
        return { 37, 48, 35, 60 };
    case SpriteId::Adrian:
        return { 0, 48, 35, 60 };
    case SpriteId::Basic1:
        return { 73, 0, 41, 41 };
    case SpriteId::Basic2:
        return { 73, 41, 41, 41 };
    case SpriteId::Basic3:
        return { 73, 82, 41, 41 };
    case SpriteId::Heart:
        return { 0, 15, 28, 25 };
    case SpriteId::Star:
        return { 28, 15, 28, 29 };
    case SpriteId::Fireball:
        return { 62, 1, 11, 20 };
    case SpriteId::Projectile1:
        return { 0, 0, 15, 15 };
    case SpriteId::Projectile2:
        return { 15, 0, 15, 15 };
    case SpriteId::Projectile3:
        return { 30, 0, 15, 15 };
    case SpriteId::Projectile4:
        return { 45, 0, 15, 15 };
    }

    assert(false && "Invalid sprite id supplied");
}

}
