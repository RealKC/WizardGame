#include "Game.h"

#include "Utils.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>

namespace WizardGame {

enum class ShouldRemove {
    Yes,
    No,
};

template<typename Item, typename Callback>
void iterate_vector_for_removing(std::vector<Item>& items, Callback cb)
{
    int i = 0;
    while (i < items.size()) {
        auto should_remove = cb(items[i]);
        if (should_remove == ShouldRemove::Yes) {
            std::swap(items[i], items.back());
            items.pop_back();
        } else {
            ++i;
        }
    }
}

Game::Game()
    : m_player(Collider { 300, 300, 50, 50 })
    , m_last_bullet_shot_time(0)
    , m_quit(false)
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

    srand((unsigned int)(uintptr_t)m_renderer);

    while (!m_quit) {
        SDL_Event event;
        uint32_t start_ticks = SDL_GetTicks();

        event_loop();

        handle_player_keypresses(start_ticks);
        update_bullet_positions();
        tick_enemies(start_ticks);
        check_collisions();

        SDL_RenderClear(m_renderer);
        SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(m_renderer);
        render_bullets();
        render_entities();
        SDL_RenderPresent(m_renderer);

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
            info() << "Quitting...";
            SDL_Quit();
            m_quit = true;
            break;
        default:
            break;
        }
    }
}

void Game::render_entities()
{
    m_player.render(m_renderer);

    for (auto& enemy : m_enemies) {
        enemy->render(m_renderer);
    }
}

void Game::render_bullets()
{
    for (auto& bullet : m_player_bullets) {
        SDL_Rect rect { bullet.position().x, bullet.position().y, bullet.size().width, bullet.size().height };
        SDL_SetRenderDrawColor(m_renderer, 0x00, 0xff, 0x00, 0xff);
        SDL_RenderFillRect(m_renderer, &rect);
    }

    for (auto& bullet : m_enemy_bullets) {
        SDL_Rect rect { bullet.position().x, bullet.position().y, bullet.size().width, bullet.size().height };
        SDL_SetRenderDrawColor(m_renderer, 0x80, 0xff, 0x00, 0xff);
        SDL_RenderFillRect(m_renderer, &rect);
    }
}

void Game::update_bullet_positions()
{
    iterate_vector_for_removing(m_player_bullets, [](auto& bullet) {
        auto has_hit_wall = bullet.move();
        if (has_hit_wall == HasHitWall::Yes) {
            return ShouldRemove::Yes;
        } else {
            return ShouldRemove::No;
        }
    });

    iterate_vector_for_removing(m_enemy_bullets, [](auto& bullet) {
        auto has_hit_wall = bullet.move();
        if (has_hit_wall == HasHitWall::Yes) {
            return ShouldRemove::Yes;
        } else {
            return ShouldRemove::No;
        }
    });
}

void Game::tick_enemies(uint32_t current_time)
{
    for (auto& enemy : m_enemies) {
        enemy->tick(m_enemy_bullets, current_time);
    }
}

void Game::check_collisions()
{
    // Check if player bullets hit an enemy

    // Check if enemy bullets hit the player
}

void Game::handle_player_keypresses(uint32_t current_time)
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

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_X)) {
        constexpr uint32_t ATTACK_COOLDOWN = 350;
        if (current_time - m_last_bullet_shot_time >= ATTACK_COOLDOWN) {
            // Allow the player to shoot a bullet
            m_player_bullets.push_back(m_player.make_bullet());
            m_last_bullet_shot_time = current_time;
        }
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_7)) {
        auto position = Vec2 { rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT };
        m_enemies.push_back(make_enemy(position, position + Vec2 { 50, 50 }));
    }
}

int Game::initialize_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error() << "Failed to initialize video: " << SDL_GetError() << std::endl;
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

    return 0;
}

}
