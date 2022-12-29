#include "FileLevel.h"

#include "../Utils.h"
#include "Enemies/Attack.h"
#include "Enemies/Basic.h"
#include <assert.h>
#include <fstream>
#include <sstream>

namespace WizardGame {

using Enemies::Attack;

static Attack::Type parse_attack(char ch)
{
    switch (ch) {
    case 'c':
        return Attack::Type::Circle;
    case 'l':
        return Attack::Type::Line;
    case 't':
        return Attack::Type::ThreeAtOnce;
    }

    assert(false && "Nu ar trebui sa ajunga aici.");
}

static int parse_number_from_directive(std::string const& s)
{
    auto first_ws = s.find_first_of(' ');
    return atoi(s.c_str() + first_ws + 1);
}

FileLevel::FileLevel(uint32_t level_event, std::string const& path)
    : Level(level_event, { 450, 450 })
    , m_wave(0)
{
    parse_level(path);
    next_wave();
}

void FileLevel::render_impl(SDL_Renderer* renderer, TextRenderer& text_renderer, SpriteManager& sprite_manager)
{
    // Call render_bullets before render_entities so bullets sent by the player spawn under them.
    render_bullets(renderer, sprite_manager);
    render_entities(renderer, sprite_manager);
}

void FileLevel::run_frame_impl(uint32_t current_time)
{
    if (m_enemies.empty()) {
        next_wave();
    }

    handle_player_keypresses(current_time);
    update_bullet_positions();
    tick_enemies(current_time);
    check_collisions();
    m_player.decrease_iframes();
}

void FileLevel::dismiss_dialogue_if_any() { }

void FileLevel::kill_player()
{
    if (m_player.has_iframes()) {
        return;
    }

    auto lost_final_life = m_player.die();

    if (lost_final_life == LostFinalLife::Yes) {
        info() << "Ha, you lost\n";
        restart_level();
    } else {
        restart_wave();
    }
}

void FileLevel::restart_level()
{
    Level::restart_level();
    m_wave = 1;
    m_enemies.clear();
    spawn_wave(m_enemies);
}

int FileLevel::next_level() const
{
    switch (m_background_id) {
    case BackgroundId::Level1:
        return 2;
    case BackgroundId::Level2:
        return 3;
    case BackgroundId::Level3:
        return -1;
    default:
        error() << "Unexpected m_background_id: " << static_cast<int>(m_background_id) << std::endl;
        assert(false);
    }
}

void FileLevel::restart_wave()
{
    m_enemies.clear();
    spawn_wave(m_enemies);
}

void FileLevel::next_wave()
{
    m_wave++;
    assert(m_enemies.empty() && "FileLevel::next_wave should only be called when there's no more enemies on the screen");
    spawn_wave(m_enemies);
}

void FileLevel::parse_level(std::string const& path)
{
    std::ifstream level(path);
    std::string line;

    size_t wave = 0;
    while (std::getline(level, line)) {
        if (line.find("wave") == 0) {
            wave = parse_number_from_directive(line);
        } else if (line.find("basic") == 0) {
            std::stringstream stream(line.substr(strlen("basic")));

            char attack_ch;
            stream >> attack_ch;
            Attack::Type attack = parse_attack(attack_ch);

            int x, y, w, h;
            stream >> x >> y >> w >> h;
            Collider collider { x, y, w, h };

            stream >> x >> y;
            Vec2 target_position { x, y };

            m_enemy_infos.push_back(EnemyData {
                wave,
                collider,
                target_position,
                attack,
                true });
        } else if (line.find("boss") == 0) {
            // TODO: Parse adrian
        } else if (line.find("background") == 0) {
            auto background = parse_number_from_directive(line);
            switch (background) {
            case 1:
                m_background_id = BackgroundId::Level1;
                break;
            case 2:
                m_background_id = BackgroundId::Level2;
                break;
            case 3:
                m_background_id = BackgroundId::Level3;
                break;
            default:
                error() << "Unknown background: '" << background << "'\n";
                assert(false);
            }
        } else if (line.find("title") == 0) {
            set_title(line.substr(line.find(' ') + 1));
        } else {
            error() << "Cannot parse line: '" << line << "'\n";
        }
    }
    m_final_wave = wave + 1;
}

void FileLevel::spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies)
{
    for (auto& enemy : m_enemy_infos) {
        if (m_wave == enemy.wave) {
            if (enemy.is_basic) {
                Attack attack { enemy.attack, 1200, 0 };
                enemies.push_back(std::make_unique<Enemies::Basic>(enemy.collider, enemy.target_position, std::vector { attack }));
            } else {
                // TODO: Adrian!
            }
        }
    }
}

}
