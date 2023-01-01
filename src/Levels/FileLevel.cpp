#include "FileLevel.h"

#include "../Utils.h"
#include "Enemies/Adrian.h"
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

static Enemies::Adrian const* get_adrian(std::vector<std::unique_ptr<Enemy>> const& enemies)
{
    if (enemies.size() != 1) {
        return nullptr;
    }

    return dynamic_cast<Enemies::Adrian const*>(enemies[0].get());
}

FileLevel::FileLevel(uint32_t level_event, std::string const& path)
    : Level(level_event, { 450, 450 })
    , m_wave(0)
    , m_adrian_wave(0)
    , m_has_shown_mid_boss_dialog(true)
    , m_has_shown_end_dialog(true)
{
    parse_level(path);
    next_wave();
}

void FileLevel::render_impl(SDL_Renderer* renderer, TextRenderer& text_renderer, SpriteManager& sprite_manager)
{
    // Call render_bullets before render_entities so bullets sent by the player spawn under them.
    render_bullets(sprite_manager);
    render_entities(sprite_manager);

    if (!m_has_shown_mid_boss_dialog) {
        auto* adrian = get_adrian(m_enemies);
        if (adrian && adrian->has_pending_dialog()) {
            render_dialog(renderer, text_renderer, sprite_manager, PortraitId::Adrian, "Adrian", adrian->dialog_text());
        }
    } else if (m_speaker.length() != 0 && m_wave == m_final_wave && !m_has_shown_end_dialog) {
        render_dialog(renderer, text_renderer, sprite_manager, m_portrait_id, m_speaker, m_end_dialog_text);
    }
}

void FileLevel::run_frame_impl(uint32_t current_time)
{
    if (!m_has_shown_mid_boss_dialog) {
        auto* adrian = get_adrian(m_enemies);
        if (adrian && adrian->has_pending_dialog()) {
            return;
        }
    }

    if (m_enemies.empty()) {
        next_wave();
    }

    handle_player_keypresses(current_time);
    update_bullet_positions();
    tick_enemies(current_time);
    check_collisions();
    m_player.decrease_iframes();
}

void FileLevel::dismiss_dialogue_if_any()
{
    if (m_has_shown_mid_boss_dialog && m_wave == m_final_wave) {
        m_has_shown_end_dialog = true;
    } else if (auto* adrian = get_adrian(m_enemies); adrian && adrian->has_pending_dialog()) {
        m_has_shown_mid_boss_dialog = true;
    }
}

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
    if (m_wave == m_final_wave) {
        return;
    }

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
            });
        } else if (line.find("adrian") == 0) {
            // Once we've reached an adrian directive, there's no going back, the rest of the file
            // details what things adrian will do
            parse_adrian(level);
            m_has_shown_mid_boss_dialog = false;
            m_adrian_wave = wave;
            break;
        } else if (line.find("background") == 0) {
            auto background = parse_number_from_directive(line);
            switch (background) {
            case 1:
                m_background_id = BackgroundId::Level1;
                set_high_score(HighScore::fetch(Save::Level::One));
                set_save_level_id(Save::Level::One);
                break;
            case 2:
                m_background_id = BackgroundId::Level2;
                set_high_score(HighScore::fetch(Save::Level::Two));
                set_save_level_id(Save::Level::Two);
                break;
            case 3:
                m_background_id = BackgroundId::Level3;
                set_high_score(HighScore::fetch(Save::Level::Three));
                set_save_level_id(Save::Level::Three);
                break;
            default:
                error() << "[GENERIC PARSE] Unknown background: '" << background << "'\n";
                assert(false);
            }
        } else if (line.find("title") == 0) {
            set_title(line.substr(line.find(' ') + 1));
        } else if (line.find("dialog") == 0) {
            m_has_shown_end_dialog = false;
            if (line.find_first_of('0') != std::string::npos) {
                m_end_dialog_text = line.substr(line.find_first_of('0') + 2);
                m_speaker = "Adrian";
                m_portrait_id = PortraitId::Adrian;
            } else if (line.find_first_of('1') != std::string::npos) {
                m_end_dialog_text = line.substr(line.find_first_of('1') + 2);
                m_speaker = "Mircea";
                m_portrait_id = PortraitId::Mircea;
            } else {
                error() << "[GENERIC PARSE] Unknown dialog directive: '" << line << "'\n";
            }
        } else {
            error() << "[GENERIC PARSE] Cannot parse line: '" << line << "'\n";
        }
    }

    m_final_wave = wave + 1;

    if (m_adrian_wave == 0) {
        m_adrian_wave = m_final_wave + 10;
    }
}

void FileLevel::parse_adrian(std::istream& in)
{
    std::string line;

    Collider collider { 450, 50, 200, 200 };
    Vec2 target_position;
    int health;
    int score;
    std::vector<Enemies::Attack> attacks;
    std::string text;
    // I doubt we'll have more attacks than this
    attacks.reserve(128);
    while (std::getline(in, line)) {
        if (line.find("line") == 0) {
            auto cooldown = parse_number_from_directive(line);
            attacks.push_back(Attack {
                Attack::Type::Line,
                cooldown,
                0 });
        } else if (line.find("three") == 0) {
            auto cooldown = parse_number_from_directive(line);
            attacks.push_back(Attack {
                Attack::Type::ThreeAtOnce,
                cooldown,
                0 });
        } else if (line.find("circle") == 0) {
            auto cooldown = parse_number_from_directive(line);
            attacks.push_back(Attack {
                Attack::Type::Circle,
                cooldown,
                0 });
        } else if (line.find("health") == 0) {
            health = parse_number_from_directive(line);
        } else if (line.find("collider") == 0) {
            std::stringstream stream(line.substr(strlen("collider")));
            int x, y, w, h;
            stream >> x >> y >> w >> h;
            collider = Collider { x, y, w, h };
        } else if (line.find("target") == 0) {
            std::stringstream stream(line.substr(strlen("target")));
            int x, y;
            stream >> x >> y;
            target_position = Vec2 { x, y };
        } else if (line.find("score") == 0) {
            score = parse_number_from_directive(line);
        } else if (line.find("dialog") == 0) {
            text = line.substr(strlen("dialog") + 1);
        } else {
            error() << "[ADRIAN PARSE] Cannot parse line: '" << line << "'\n";
        }
    }

    m_adrian_data = AdrianData {
        collider,
        target_position,
        health,
        score,
        attacks,
        text
    };
}

void FileLevel::spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (m_wave == m_adrian_wave) {
        enemies.push_back(std::make_unique<Enemies::Adrian>(
            m_adrian_data->collider,
            m_adrian_data->target_position,
            m_adrian_data->attacks,
            m_adrian_data->health,
            m_adrian_data->score,
            m_adrian_data->text));
        return;
    }

    for (auto& enemy : m_enemy_infos) {
        if (m_wave == enemy.wave) {
            Attack attack { enemy.attack, 1200, 0 };
            enemies.push_back(std::make_unique<Enemies::Basic>(enemy.collider, enemy.target_position, std::vector { attack }));
        }
    }
}
}
