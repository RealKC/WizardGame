#include "Level.h"

#include "../Game.h"
#include "../UserEvents.h"
#include "../Utils.h"

namespace WizardGame {

enum class ShouldRemove {
    Yes,
    No,
};

template<typename Item, typename Callback>
static void iterate_vector_for_removing(std::vector<Item>& items, Callback cb)
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

Level::Level(uint32_t level_event, Vec2 spawn)
    : m_level_event(level_event)
    , m_player(Collider { spawn.x, spawn.y, 10, 10 }, Size { 50, 50 })
{
    // Reserve some memory to hopefully avoid some allocations during frame code
    m_enemies.reserve(128);
    m_player_bullets.reserve(512);
    m_enemy_bullets.reserve(512);
}

void Level::restart_level()
{
    m_player.reset_lives();
    m_player.go_back_to_spawn();
}

void Level::run_frame(uint32_t current_time)
{
    if (!is_paused()) {
        run_frame_impl(current_time);
    }
}

void Level::render(SDL_Renderer* renderer, TextRenderer& text_renderer, SpriteManager& sprite_manager)
{
    sprite_manager.render_background(background_id());

    // Dimmen the playing area so it's clear to the player where they can move
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_Rect playing_area_rect {
        PLAYING_AREA_LEFT_LIMIT,
        PLAYING_AREA_TOP_LIMIT,
        PLAYING_AREA_RIGHT_LIMIT - PLAYING_AREA_LEFT_LIMIT,
        PLAYING_AREA_BOTTOM_LIMIT - PLAYING_AREA_TOP_LIMIT
    };
    SDL_RenderFillRect(renderer, &playing_area_rect);

    text_renderer.render_wrapped_big_text_at(m_title, { PLAYING_AREA_RIGHT_LIMIT + 20, 20 }, { 0, 0, 0 });

    render_impl(renderer, text_renderer, sprite_manager);

    if (is_paused()) {
        m_pause_menu->render(renderer, text_renderer);
    }
}

void Level::toggle_pause_state()
{
    if (m_pause_menu) {
        m_pause_menu = nullptr;
    } else {
        m_pause_menu = std::make_unique<UI::PauseMenu>();
    }
}

void Level::handle_key_event(SDL_KeyboardEvent keyboard_event)
{
    switch (keyboard_event.keysym.sym) {
    case SDLK_ESCAPE:
        toggle_pause_state();
        break;
    case SDLK_RETURN:
        if (is_paused()) {
            auto activation_result = m_pause_menu->activate_current_selection();
            switch (activation_result) {
            case UI::PauseMenu::Continue:
                toggle_pause_state();
                break;
            case UI::PauseMenu::Restart:
                restart_level();
                toggle_pause_state();
                break;
            case UI::PauseMenu::QuitToMenu: {
                SDL_Event event;
                SDL_zero(event);
                event.type = m_level_event;
                event.user.code = LevelEvents::QuitToMenu;
                SDL_PushEvent(&event);
                break;
            }
            case UI::PauseMenu::QuitToDesktop: {
                SDL_Event event;
                SDL_zero(event);
                event.type = m_level_event;
                event.user.code = LevelEvents::QuitToDesktop;
                SDL_PushEvent(&event);
                break;
            }
            }
        } else {
            dismiss_dialogue_if_any();
        }
        break;
    case SDLK_UP:
        if (is_paused()) {
            m_pause_menu->select_previous_item();
        }
        break;
    case SDLK_DOWN:
        if (is_paused()) {
            m_pause_menu->select_next_item();
        }
        break;
    }
}

void Level::render_entities(SDL_Renderer* renderer, SpriteManager& sprite_manager)
{
    m_player.render(renderer, sprite_manager);

    for (auto& enemy : m_enemies) {
        enemy->render(renderer, sprite_manager);
    }
}

void Level::render_bullets(SDL_Renderer* renderer, SpriteManager&)
{
    for (auto& bullet : m_player_bullets) {
        SDL_Rect rect { bullet.position().x, bullet.position().y, bullet.size().width, bullet.size().height };
        SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
        SDL_RenderFillRect(renderer, &rect);
    }

    for (auto& bullet : m_enemy_bullets) {
        SDL_Rect rect { bullet.position().x, bullet.position().y, bullet.size().width, bullet.size().height };
        SDL_SetRenderDrawColor(renderer, 0x80, 0xff, 0x00, 0xff);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Level::render_dialog(SDL_Renderer* renderer, TextRenderer& text_renderer, SpriteManager& sprite_manager, PortraitId portrait_id, std::string const& speaker, std::string const& speech)
{
    int x = 20;
    int y = 3 * Game::WINDOW_HEIGHT / 5;

    auto portrait_size = size_for_portrait_id(portrait_id);

    int dialog_height = portrait_size.height + 20;
    int dialog_width = Level::PLAYING_AREA_RIGHT_LIMIT - 100;

    if (text_renderer.measure_big_text(speaker) > dialog_width) {
        dialog_width = Game::WINDOW_WIDTH;
    }

    int dialog_text_width = dialog_width - portrait_size.width - 100;

    SDL_SetRenderDrawColor(renderer, 245, 245, 220, 75);
    SDL_Rect dialog_rect { x - 10, y - 10, dialog_width, dialog_height };
    SDL_RenderFillRect(renderer, &dialog_rect);

    sprite_manager.render_portrait_at(portrait_id, { x, y });
    x += 20 + portrait_size.width;
    y += text_renderer.render_big_text_at(speaker, { x, y }, { 0, 0, 0 }).height;
    y += 20;
    text_renderer.render_wrapped_regular_text_at(speech, { x, y }, { 0, 0, 0 }, dialog_text_width);
}

void Level::update_bullet_positions()
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

void Level::check_collisions()
{
    // Check if the player hit an enemy
    bool should_clear_all_bullets = false;
    for (std::size_t i = 0; i < m_enemies.size(); ++i) {
        if (m_player.collides_with(*m_enemies[i])) {
            kill_player();
            should_clear_all_bullets = true;
            break;
        }
    }

    // Check if player bullets hit an enemy
    iterate_vector_for_removing(m_player_bullets, [&](auto& bullet) {
        bool bullet_is_to_be_removed = false;
        iterate_vector_for_removing(m_enemies, [&](auto& enemy) {
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
            should_clear_all_bullets = true;
            return ShouldRemove::Yes;
        }

        return ShouldRemove::No;
    });

    if (should_clear_all_bullets) {
        m_player_bullets.clear();
        m_enemy_bullets.clear();
    }
}

void Level::tick_enemies(uint32_t current_time)
{
    for (auto& enemy : m_enemies) {
        enemy->tick(m_enemy_bullets, current_time);
    }
}

void Level::handle_player_keypresses(uint32_t current_time)
{
    constexpr int STEP = 2;

    int delta_x = 0;
    int delta_y = 0;

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_W) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_UP)) {
        scancode_hook(SDL_SCANCODE_UP);
        delta_y -= STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_S) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_DOWN)) {
        scancode_hook(SDL_SCANCODE_DOWN);
        delta_y += STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_A) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_LEFT)) {
        scancode_hook(SDL_SCANCODE_LEFT);
        delta_x -= STEP;
    }

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_D) || m_keyboard_state.is_key_pressed(SDL_SCANCODE_RIGHT)) {
        scancode_hook(SDL_SCANCODE_RIGHT);
        delta_x += STEP;
    }

    m_player.move_by(delta_x, delta_y);

    if (m_keyboard_state.is_key_pressed(SDL_SCANCODE_X)) {
        scancode_hook(SDL_SCANCODE_X);
        constexpr uint32_t ATTACK_COOLDOWN = 150;
        if (current_time - m_last_bullet_shot_time >= ATTACK_COOLDOWN) {
            // Allow the player to shoot a bullet
            m_player_bullets.push_back(m_player.make_bullet());
            m_last_bullet_shot_time = current_time;
        }
    }
}

}
