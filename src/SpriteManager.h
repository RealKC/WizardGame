#pragma once

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

class SpriteManager {
public:
    explicit SpriteManager(SDL_Renderer*);
    ~SpriteManager();

    void render_sprite_for_id_at_position(SpriteId, Vec2 position);

    void render_background(BackgroundId) const;

private:
    static constexpr std::size_t BACKGROUND_COUNT = static_cast<std::size_t>(BackgroundId::Level3) + 1;
    SDL_Rect sprite_id_to_source_rect(SpriteId);

    SDL_Texture* m_texture;
    SDL_Texture* m_backgrounds[BACKGROUND_COUNT];
    SDL_Renderer* m_renderer;
};

}
