#include "Level.h"

#include "../Game.h"
#include "../UI/GameOverMenu.h"
#include "../UI/LevelCompletedMenu.h"
#include "../UI/PauseMenu.h"
#include "../UserEvents.h"
#include "../Utils.h"
#include "Enemies/Adrian.h"
#include <assert.h>

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

        if (items.empty() || i > items.size()) {
            // If the callback mutates our vector, we should try to tolerate it, we'll also make some noise about it though!
            error() << "[SUS] `items` mutated while iterating through it in " << __FUNCTION__ << std::endl;
            return;
        }

        if (should_remove == ShouldRemove::Yes) {
            std::swap(items[i], items.back());
            items.pop_back();
        } else {
            ++i;
        }
    }
}

Level::HighScore Level::HighScore::fetch(Save::Level level)
{
    auto value = Save::get_high_score_for(level);
    auto string = std::to_string(value);
    if (string.length() < 8) {
        string = std::string(8 - string.length(), '0') + string;
    }

    return { value, string };
}

Level::Level(uint32_t level_event, Vec2 spawn)
    : m_level_event(level_event)
    , m_player(Collider { spawn.x, spawn.y, 10, 10 }, Size { 50, 50 })
    , m_last_bullet_shot_time(0)
    , m_score(0)
{
    // Reserve some memory to hopefully avoid some allocations during frame code
    m_enemies.reserve(128);
    m_player_bullets.reserve(512);
    m_enemy_bullets.reserve(512);
}

Level::~Level()
{
    if (has_high_score()) {
        Save::set_high_score_for(m_save_level_id, m_score);
    }

    auto last_beat_level = Save::get_last_beat_level();
    if ((int)last_beat_level < (int)m_save_level_id) {
        Save::set_last_beat_level(m_save_level_id);
    }
}

void Level::restart_level()
{
    if (m_player.is_dead()) {
        m_menu = std::make_unique<UI::GameOverMenu>();
    }
    m_player.reset_lives();
    m_player.go_back_to_spawn();
    m_score = 0;
}

void Level::run_frame(uint32_t current_time)
{
    if (!m_menu) {
        run_frame_impl(current_time);
    }

    if (has_been_won() && !m_menu) {
        m_menu = std::make_unique<UI::LevelCompletedMenu>(has_next_level(), has_high_score());
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

    int x = PLAYING_AREA_RIGHT_LIMIT + 20;
    int y = text_renderer
                .render_wrapped_big_text_at(m_title, { x, 20 }, { 0, 0, 0 })
                .height;
    y += 20;
    auto score_string = std::to_string(m_score);
    if (score_string.length() < 8) {
        score_string = std::string(8 - score_string.length(), '0') + score_string;
    }
    y += text_renderer.render_regular_text_at("     Score: " + score_string, { x, y }, { 0, 0, 0 }).height;
    y += 20;
    text_renderer.render_regular_text_at("High Score: " + m_high_score.string, { x, y }, { 0, 0, 0 });

    if (auto boss_health = this->boss_health(); boss_health != -1) {
        int health_x = x + (Game::WINDOW_WIDTH - (PLAYING_AREA_RIGHT_LIMIT - PLAYING_AREA_LEFT_LIMIT)) / 2 - 100;
        int health_y = y + 100;
        int width = 150;
        int red_width = width * boss_health;
        int height = 60;

        SDL_Rect black_rect { health_x, health_y, width, height };
        SDL_Rect red_rect { health_x, health_y, red_width, height };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
        SDL_RenderFillRect(renderer, &black_rect);
        SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
        SDL_RenderFillRect(renderer, &red_rect);

        sprite_manager.render_frame_at({ health_x - 25, health_y - 60 });
    }

    y = Game::WINDOW_HEIGHT - Game::WINDOW_HEIGHT / 10;
    auto heart_size = sprite_manager.render_sprite_for_id_at_position(SpriteId::Heart, { x, y }, 2);
    x += heart_size.width;

    text_renderer.render_big_text_at("x" + std::to_string(m_player.lives()), { x, y }, { 255, 0, 0 });

    render_impl(renderer, text_renderer, sprite_manager);

    if (m_menu) {
        m_menu->render(renderer, text_renderer);
    }
}

void Level::toggle_pause_state()
{
    if (is_paused()) {
        m_menu = nullptr;
    } else if (!m_menu) {
        m_menu = std::make_unique<UI::PauseMenu>();
    }
}

void Level::handle_key_event(SDL_KeyboardEvent keyboard_event)
{
    switch (keyboard_event.keysym.sym) {
    case SDLK_ESCAPE:
        toggle_pause_state();
        break;
    case SDLK_RETURN: {
        auto quit_to_menu = [&] {
            SDL_Event event;
            SDL_zero(event);
            event.type = m_level_event;
            event.user.code = LevelEvents::QuitToMenu;
            SDL_PushEvent(&event);
        };

        auto quit_to_desktop = [&] {
            SDL_Event event;
            SDL_zero(event);
            event.type = m_level_event;
            event.user.code = LevelEvents::QuitToDesktop;
            SDL_PushEvent(&event);
        };

        auto next_level = [&](std::intptr_t level) {
            SDL_Event event;
            SDL_zero(event);
            event.type = m_level_event;
            event.user.code = LevelEvents::NextLevel;
            event.user.data1 = (void*)level;
            SDL_PushEvent(&event);
        };

        if (m_menu) {
            switch (m_menu->activate_current_selection()) {
            case UI::PauseMenu::Continue:
                toggle_pause_state();
                break;
            case UI::PauseMenu::Restart:
                restart_level();
                toggle_pause_state();
                break;
            case UI::GameOverMenu::Restart:
                m_menu = nullptr;
                break;
            case UI::LevelCompletedMenu::NextLevel:
                assert(has_next_level());
                next_level(this->next_level());
                break;
            case UI::AbstractLevelMenu::QuitToMenu:
                quit_to_menu();
                break;
            case UI::AbstractLevelMenu::QuitToDesktop:
                quit_to_desktop();
                break;
            }
        } else {
            dismiss_dialogue_if_any();
        }
        break;
    }
    case SDLK_UP:
        if (m_menu) {
            m_menu->select_previous_item();
        }
        break;
    case SDLK_DOWN:
        if (m_menu) {
            m_menu->select_next_item();
        }
        break;
#ifdef DEBUG_GAME_MENUS
    case SDLK_j:
        m_menu = std::make_unique<UI::LevelCompletedMenu>(has_next_level(), has_high_score());
        break;
    case SDLK_k:
        m_menu = std::make_unique<UI::PauseMenu>();
        break;
    case SDLK_l:
        m_menu = std::make_unique<UI::GameOverMenu>();
        break;
#endif
    }
}

void Level::render_entities(SDL_Renderer* renderer, SpriteManager& sprite_manager)
{
    m_player.render(renderer, sprite_manager);

    for (auto& enemy : m_enemies) {
        enemy->render(renderer, sprite_manager);
    }
}

void Level::render_bullets(SpriteManager& sprite_manager)
{
    for (auto& bullet : m_player_bullets) {
        bullet.render(sprite_manager);
    }

    for (auto& bullet : m_enemy_bullets) {
        bullet.render(sprite_manager);
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
    bool player_died = false;
    for (std::size_t i = 0; i < m_enemies.size(); ++i) {
        if (m_player.collides_with(*m_enemies[i])) {
            kill_player();
            player_died = true;
            break;
        }
    }

    // Check if player bullets hit an enemy
    iterate_vector_for_removing(m_player_bullets, [&](auto& bullet) {
        bool bullet_is_to_be_removed = false;
        iterate_vector_for_removing(m_enemies, [&](auto& enemy) {
            if (enemy->collides_with(bullet)) {
                bullet_is_to_be_removed = true;
                if (enemy->hit() == Enemy::Died::Yes) {
                    increase_score_by(1, enemy->score_value() - m_player.has_iframes(), m_player.score_value());
                    return ShouldRemove::Yes;
                }
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
            player_died = true;
            return ShouldRemove::Yes;
        }

        return ShouldRemove::No;
    });

    if (player_died) {
        m_score /= 2;
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
        constexpr uint32_t ATTACK_COOLDOWN = 200;
        if (current_time - m_last_bullet_shot_time >= ATTACK_COOLDOWN) {
            // Allow the player to shoot a bullet
            m_player_bullets.push_back(m_player.make_bullet());
            m_last_bullet_shot_time = current_time;
        }
    }
}

float Level::boss_health() const
{
    if (m_enemies.size() != 1) {
        return -1;
    }

    if (auto* adrian = dynamic_cast<Enemies::Adrian*>(m_enemies[0].get()); adrian != nullptr) {
        return adrian->health_percentage();
    }

    return -1;
}

}
