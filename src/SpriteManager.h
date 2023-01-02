#pragma once

#include "Size.h"
#include "Vec2.h"
#include <SDL2/SDL.h>

namespace WizardGame {

enum class SpriteId {
    Player,
    Adrian,
    Basic1,
    Basic2,
    Basic3,
    Heart,
    Star,
    Fireball,
    Projectile1,
    Projectile2,
    Projectile3,
    Projectile4,
};

enum class BackgroundId {
    Menu,
    Tutorial,
    Level1,
    Level2,
    Level3,
};

enum class PortraitId {
    Adrian,
    Matei,
    Mihai,
    Mircea,
};

inline Size size_for_portrait_id(PortraitId)
{
    return { 200, 200 };
}

class SpriteManager {
public:
    explicit SpriteManager(SDL_Renderer*);
    ~SpriteManager();

    Size render_sprite_for_id_at_position(SpriteId, Vec2 position, int scale = 1);

    void render_frame_at(Vec2 position) const;
    void render_portrait_at(PortraitId, Vec2 position) const;
    void render_background(BackgroundId) const;

    static Size size_for_sprite_id(SpriteId sprite_id)
    {
        auto rect = sprite_id_to_source_rect(sprite_id);

        return { rect.w, rect.h };
    }

private:
    static constexpr std::size_t BACKGROUND_COUNT = static_cast<std::size_t>(BackgroundId::Level3) + 1;
    static constexpr std::size_t PORTRAIT_COUNT = static_cast<std::size_t>(PortraitId::Mircea) + 1;

    static SDL_Rect sprite_id_to_source_rect(SpriteId);

    SDL_Texture* m_texture;
    SDL_Texture* m_backgrounds[BACKGROUND_COUNT];
    SDL_Texture* m_portraits[PORTRAIT_COUNT];
    SDL_Texture* m_frame;

    SDL_Renderer* m_renderer;
};

}
