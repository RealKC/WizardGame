#include "Game.h"

#include "Utils.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace WizardGame {

Game::Game()
    : m_player(Vec2 { 300, 300 })
    , m_window(nullptr)
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
        case SDL_KEYUP:
            break;
        default:
            info() << "Unhandled event type: " << event.type << std::endl;
            break;
        }

        handle_player_movement();

        SDL_RenderClear(m_renderer);
        SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(m_renderer);
        render_entities();
    }
}

void Game::render_entities()
{
    SDL_SetRenderDrawColor(m_renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_Rect player_rect { m_player.position().x, m_player.position().y, 50, 50 };
    SDL_RenderFillRect(m_renderer, &player_rect);
    SDL_RenderPresent(m_renderer);
}

void Game::handle_keyboard_event(SDL_KeyboardEvent keyboard_event)
{

    int delta_x = 0;
    int delta_y = 0;
    auto key = keyboard_event.keysym.sym;
}

void Game::handle_player_movement()
{
    constexpr int STEP = 2;

    int delta_x = 0;
    int delta_y = 0;

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_W)) {
        delta_y -= STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_S)) {
        delta_y += STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_A)) {
        delta_x -= STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_D)) {
        delta_x += STEP;
    }

    m_player.move_by(delta_x, delta_y);
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
