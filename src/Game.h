#pragma once

#include "Bullet.h"
#include "KeyboardState.h"
#include "Player.h"
#include <SDL2/SDL.h>
#include <vector>

namespace WizardGame {

class Game {
public:
    Game();
    ~Game();

    int run();

private:
    int initialize_sdl();

    void event_loop();
    void handle_keyboard_event(SDL_KeyboardEvent);
    void handle_player_movement();

    void render_entities();
    void render_bullets();

    void update_bullet_positions();

    // Game state
    Player m_player;
    std::vector<Bullet> m_bullets;
    bool m_quit;

    // Render state
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    KeyboardState m_keyboard_state;
};

}
