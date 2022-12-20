#include "Game.h"

#include "Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdlib.h>

namespace WizardGame {

Game::Game()
    : m_quit(false)
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

    TTF_Quit();
    SDL_Quit();
}

int Game::run()
{
    if (auto rc = initialize_sdl(); rc != RES_OK) {
        return rc;
    }

    if (auto rc = m_sprite_manager.initialize(m_renderer); rc != RES_OK) {
        return rc;
    }

    if (auto rc = m_text_renderer.initialize(m_renderer); rc != RES_OK) {
        return rc;
    }

    srand((unsigned int)(uintptr_t)m_renderer);

    m_level = std::make_unique<Level>();
    m_level->load("resources/level1.txt");
    m_level->dump();

    while (!m_quit) {
        uint32_t start_ticks = SDL_GetTicks();

        event_loop();

        if (m_level) {
            m_level->run_frame(start_ticks);

            SDL_RenderClear(m_renderer);
            SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderClear(m_renderer);
            m_sprite_manager.render_sprite_for_id_at_position(SpriteId::Player, { 0, 0 });
            m_level->render(m_renderer);
            m_text_renderer.render_text_at("Hello world", { 100, 100 });
            SDL_RenderPresent(m_renderer);
        }

        uint32_t end_ticks = SDL_GetTicks();

        uint32_t passed_time = end_ticks - start_ticks;
        if (passed_time > 16) {
            // we're lagging, don't do anything
            error() << "WE'RE LAGGING!!!!\n";
        } else {
            // Lock the framerate to 60fps
            SDL_Delay(16 - passed_time);
        }
    }

    return 0;
}

void Game::event_loop()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            info() << "Quitting..." << std::endl;
            SDL_Quit();
            m_quit = true;
            break;
        case SDL_KEYDOWN: {
            break;
        }
        default:
            break;
        }
    }
}

int Game::initialize_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error() << "Failed to initialize video: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() < 0) {
        error() << "Failed to initialize SDL2_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    m_window = SDL_CreateWindow(
        "Wizard Game",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,

        SDL_WINDOW_SHOWN);
    if (!m_window) {
        error() << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        return 2;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        error() << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        return 3;
    }

    return RES_OK;
}

}
