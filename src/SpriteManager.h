#pragma once

#include "Size.h"
#include "Vec2.h"
#include <SDL2/SDL.h>

namespace WizardGame {

enum class SpriteId {
    Player,
    Matei,
    Mihai,
    Adrian,
    Basic1,
    Basic2,
    Basic3,
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

    void render_sprite_for_id_at_position(SpriteId, Vec2 position);

    void render_portrait_at(PortraitId, Vec2 position) const;
    void render_background(BackgroundId) const;

private:
    static constexpr std::size_t BACKGROUND_COUNT = static_cast<std::size_t>(BackgroundId::Level3) + 1;
    static constexpr std::size_t PORTRAIT_COUNT = static_cast<std::size_t>(PortraitId::Mircea) + 1;

    SDL_Rect sprite_id_to_source_rect(SpriteId);

    SDL_Texture* m_texture;
    SDL_Texture* m_backgrounds[BACKGROUND_COUNT];
    SDL_Texture* m_portraits[PORTRAIT_COUNT];
    SDL_Renderer* m_renderer;
};

}
