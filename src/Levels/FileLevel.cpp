#include "FileLevel.h"

#include "../Enemies/Attack.h"
#include "../Enemies/Basic.h"
#include "../Utils.h"
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

FileLevel::FileLevel(uint32_t level_event, std::string const& path)
    : Level(level_event)
    , m_wave(1)
{
    parse_level(path);
}

void FileLevel::render_impl(SDL_Renderer* renderer, TextRenderer& text_renderer)
{
    render_entities(renderer);
    render_bullets(renderer);
}

void FileLevel::run_frame_impl(uint32_t current_time)
{
    if (m_enemies.empty()) {
        next_wave();
        spawn_wave(m_enemies);
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
        info() << "Player has iframes\n";
        return;
    }

    auto lost_final_life = m_player.die();

    if (lost_final_life == LostFinalLife::Yes) {
        info() << "Ha, you lost\n";
        reset_wave();
    } else {
        previous_wave();
    }
}

void FileLevel::parse_level(std::string const& path)
{
    std::ifstream level(path);
    std::string line;

    size_t wave = 0;
    while (std::getline(level, line)) {
        if (line.find("wave") == 0) {
            wave = atoi(line.c_str() + 4);
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
            info() << "Target position is: " << target_position.to_string() << std::endl;

            m_enemy_infos.push_back(EnemyData {
                wave,
                collider,
                target_position,
                attack,
                true });
        } else if (line.find("boss") == 0) {
            // TODO: Parse adrian
        } else {
            error() << "Cannot parse line: '" << line << "'\n";
        }
    }
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
