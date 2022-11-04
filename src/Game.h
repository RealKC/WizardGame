#pragma once

#include "KeyboardState.h"
#include "Player.h"
#include <SDL2/SDL.h>

namespace WizardGame {

class Game {
public:
    Game();
    ~Game();

    int run();

private:
    int initialize_sdl();

    void handle_keyboard_event(SDL_KeyboardEvent);
    void handle_player_movement();
    void render_entities();

    // Game state
    Player m_player;

    // Render state
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    KeyboardState m_keyboard_state;
};

}
