#pragma once

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

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

}
