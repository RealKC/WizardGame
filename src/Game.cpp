#include "Game.h"

#include "Exceptions/InitError.h"
#include "Exceptions/SDLObjectError.h"
#include "UserEvents.h"
#include "Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>

namespace WizardGame {

static SDL_Window* create_window()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw InitError("SDL Video", SDL_GetError());
    }

    if (TTF_Init() < 0) {
        throw InitError("SDL2_ttf", TTF_GetError());
    }

    auto* window = SDL_CreateWindow(
        "Wizard Game",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window) {
        throw SDLObjectError("Game/SDL_Window", FailureTo::Create, "SDL window");
    }

    return window;
}

static SDL_Renderer* create_renderer(SDL_Window* window)
{
    auto* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw SDLObjectError("Game/SDL_Renderer", FailureTo::Create, "SDL renderer");
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return renderer;
}

Game::Game()
    : m_level_event(SDL_RegisterEvents(1))
    , m_quit(false)
    , m_window(create_window())
    , m_renderer(create_renderer(m_window))
    , m_sprite_manager(m_renderer)
    , m_text_renderer(m_renderer)
{
    assert(m_level_event != (Uint32)-1 && "We shouldn't have ran out of user events before even registering one");
}

Game::~Game()
{
    m_text_renderer.close_fonts();

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
    srand((unsigned int)(uintptr_t)m_renderer);

    while (!m_quit) {
        uint32_t start_ticks = SDL_GetTicks();

        event_loop();
        render(start_ticks);

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
        case SDL_KEYDOWN:
            if (is_displaying_menu()) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_LEFT:
                    m_menu.select_previous_item();
                    break;
                case SDLK_DOWN:
                case SDLK_RIGHT:
                    m_menu.select_next_item();
                    break;
                case SDLK_RETURN: {
                    auto result = m_menu.activate_current_selection();
                    switch (result) {
                    case UI::MainMenu::ActivationResult::Tutorial:
                    case UI::MainMenu::ActivationResult::Level1:
                        m_level = std::make_unique<Level>(m_level_event);
                        m_level->load("resources/level1.txt");
                        m_level->dump();
                        break;
                    case UI::MainMenu::ActivationResult::Level2:
                        m_level = std::make_unique<Level>(m_level_event);
                        m_level->load("resources/level2.txt");
                        m_level->dump();
                        break;
                    case UI::MainMenu::ActivationResult::Level3:
                        m_level = std::make_unique<Level>(m_level_event);
                        m_level->load("resources/level3.txt");
                        m_level->dump();
                        break;
                    case UI::MainMenu::ActivationResult::Quit:
                        m_quit = true;
                        break;
                    case UI::MainMenu::ActivationResult::None:
                    default:
                        break;
                    }
                    break;
                }
                case SDLK_ESCAPE:
                    m_menu.deactivate_current_selection();
                    break;
                }
            } else {
                m_level->handle_key_event(event.key);
            }
            break;
        case SDL_USEREVENT:
            assert(m_level && "We shouldn't be sending user events outside of a level");
            switch (event.user.code) {
            case LevelEvents::QuitToMenu:
                m_level = nullptr;
                break;
            case LevelEvents::QuitToDesktop:
                m_quit = true;
                break;
            }
        default:
            break;
        }
    }
}

void Game::render(uint32_t start_ticks)
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(m_renderer);
    try {
        if (m_level) {
            m_level->run_frame(start_ticks);

            m_sprite_manager.render_sprite_for_id_at_position(SpriteId::Player, { 0, 0 });
            m_level->render(m_renderer, m_text_renderer);
        } else {
            m_menu.render(m_renderer, m_text_renderer);
        }
    } catch (Exception const& except) {
        error() << "Got exception while rendering: " << except.what();
    }
    SDL_RenderPresent(m_renderer);
}

}
