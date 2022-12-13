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

class SpriteManager {
public:
    SpriteManager();
    ~SpriteManager();

    int initialize(SDL_Renderer*);

    void render_sprite_for_id_at_position(SpriteId, Vec2 position);

private:
    SDL_Rect sprite_id_to_source_rect(SpriteId);

    SDL_Texture* m_texture;
    SDL_Renderer* m_renderer;
};

}
