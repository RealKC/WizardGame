#pragma once

#include "Bullet.h"
#include "Enemy.h"
#include "KeyboardState.h"
#include "Level.h"
#include "Player.h"
#include "SpriteManager.h"
#include "TextRenderer.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace WizardGame {

class Game {
public:
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 960;

    Game();
    ~Game();

    int run();

private:
    int initialize_sdl();

    void event_loop();
    // Game state
    std::unique_ptr<Level> m_level;

    bool m_quit;

    // Render state
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SpriteManager m_sprite_manager;
    TextRenderer m_text_renderer;
};

}
