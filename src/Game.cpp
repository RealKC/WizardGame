#include "Game.h"

#include "Utils.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace WizardGame {

Game::Game()
    : m_window(nullptr)
    , m_renderer(nullptr)
{
}

Game::~Game()
{
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
}

int Game::run()
{
    if (auto rc = initialize_sdl(); rc != 0) {
        return rc;
    }

    while (true) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            info() << "Quitting...";
            SDL_Quit();
            return 0;
        case SDL_KEYDOWN:
            handle_keyboard_event(event.key);
            break;
        default:
            info() << "Unhandled event type: " << event.type << std::endl;
            break;
        }

        SDL_RenderClear(m_renderer);
        SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderPresent(m_renderer);
    }
}

void Game::handle_keyboard_event(SDL_KeyboardEvent keyboard_event)
{
    switch (keyboard_event.keysym.sym) {
    case SDLK_a:
        info() << "Key a\n";
        break;
    case SDLK_w:
        info() << "Key w\n";
        break;
    case SDLK_s:
        info() << "Key s\n";
        break;
    case SDLK_d:
        info() << "Key d\n";
        break;
    }
}

int Game::initialize_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error() << "Failed to initialize video: " << SDL_GetError() << std::endl;
        return 1;
    }

    m_window = SDL_CreateWindow("Wizard Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!m_window) {
        error() << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        return 2;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        error() << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        return 3;
    }

    return 0;
}

}
