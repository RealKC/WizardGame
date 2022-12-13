#include "Game.h"

#include "Enemies/Basic.h"
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
    : m_player(Collider { 300, 300, 10, 10 }, Size { 50, 50 })
    , m_last_bullet_shot_time(0)
    , m_level_manager()
    , m_quit(false)
    , m_window(nullptr)
    , m_renderer(nullptr)

{
    // Reserve some memory to hopefully avoid some allocations during frame code
    m_enemies.reserve(128);
    m_player_bullets.reserve(512);
    m_enemy_bullets.reserve(512);
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
    if (auto rc = initialize_sdl(); rc != RES_OK) {
        return rc;
    }

    if (auto rc = m_sprite_manager.initialize(m_renderer); rc != RES_OK) {
        return rc;
    }

    srand((unsigned int)(uintptr_t)m_renderer);

    m_level_manager.load("resources/level1.txt");
    m_level_manager.dump();

    while (!m_quit) {
        uint32_t start_ticks = SDL_GetTicks();

        event_loop();

        if (m_enemies.empty()) {
            m_level_manager.next_wave();
            m_level_manager.spawn_wave(m_enemies);
        }

        handle_player_keypresses(start_ticks);
        update_bullet_positions();
        tick_enemies(start_ticks);
        check_collisions();
        m_player.decrease_iframes();

        SDL_RenderClear(m_renderer);
        SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(m_renderer);
        m_sprite_manager.render_sprite_for_id_at_position();
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
        case SDL_KEYDOWN: {
            break;
        }
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
    // Check if the player hit an enemy
    for (auto& enemy : m_enemies) {
        if (m_player.collides_with(*enemy)) {
            kill_player();
            break;
        }
    }

    // Check if player bullets hit an enemy
    iterate_vector_for_removing(m_player_bullets, [&](auto& bullet) {
        bool bullet_is_to_be_removed = false;
        iterate_vector_for_removing(m_enemies, [&](auto& enemy) {
            info() << enemy->collides_with(bullet) << std::endl;
            if (enemy->collides_with(bullet)) {
                bullet_is_to_be_removed = true;
                return ShouldRemove::Yes;
            }
            return ShouldRemove::No;
        });

        if (bullet_is_to_be_removed) {
            return ShouldRemove::Yes;
        }

        return ShouldRemove::No;
    });

    // Check if enemy bullets hit the player
    iterate_vector_for_removing(m_enemy_bullets, [&](auto& bullet) {
        if (m_player.collides_with(bullet)) {
            kill_player();
            return ShouldRemove::Yes;
        }

        return ShouldRemove::No;
    });
}

void Game::handle_player_keypresses(uint32_t current_time)
{
    constexpr int STEP = 2;

    int delta_x = 0;
    int delta_y = 0;

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_W) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_UP)) {
        delta_y -= STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_S) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_DOWN)) {
        delta_y += STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_A) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_LEFT)) {
        delta_x -= STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_D) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_RIGHT)) {
        delta_x += STEP;
    }

    m_player.move_by(delta_x, delta_y);

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_X)) {
        constexpr uint32_t ATTACK_COOLDOWN = 150;
        if (current_time - m_last_bullet_shot_time >= ATTACK_COOLDOWN) {
            // Allow the player to shoot a bullet
            m_player_bullets.push_back(m_player.make_bullet());
            m_last_bullet_shot_time = current_time;
        }
    }
}

void Game::kill_player()
{
    if (m_player.has_iframes()) {
        info() << "Player has iframes\n";
        return;
    }

    auto lost_final_life = m_player.die();

    if (lost_final_life == LostFinalLife::Yes) {
        info() << "Ha, you lost\n";
        m_level_manager.reset_wave();
    } else {
        m_level_manager.previous_wave();
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

    return RES_OK;
}

}
