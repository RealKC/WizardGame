#pragma once

#include "Level.h"
#include <istream>
#include <optional>
#include <stdint.h>

namespace WizardGame {

class FileLevel final : public Level {
public:
    FileLevel(uint32_t level_event, std::string const& path);
    virtual ~FileLevel() { }

private:
    // ^Level
    virtual void render_impl(SDL_Renderer*, TextRenderer&, SpriteManager&) override;
    virtual void run_frame_impl(uint32_t current_time) override;
    virtual void dismiss_dialogue_if_any() override;
    virtual BackgroundId background_id() const override { return m_background_id; }
    virtual void kill_player() override;
    virtual void restart_level() override;
    virtual int next_level() const override;
    virtual bool has_been_won() const override { return m_wave == m_final_wave && m_has_shown_end_dialog; };

    void spawn_wave(std::vector<std::unique_ptr<Enemy>>& enemies);
    void restart_wave();
    void next_wave();

    void parse_level(std::string const& path);
    void parse_adrian(std::istream&);

    struct AdrianData {
        Collider collider;
        Vec2 target_position;
        int health;
        int score;
        int phase;
        std::vector<Enemies::Attack> attacks;
        std::string text;
    };

    struct EnemyData {
        size_t wave;
        Collider collider;
        Vec2 target_position;
        Enemies::Attack::Type attack;
    };

    BackgroundId m_background_id;
    size_t m_wave;
    size_t m_adrian_wave;
    size_t m_final_wave;
    std::vector<EnemyData> m_enemy_infos;
    std::optional<AdrianData> m_adrian_data;

    std::string m_speaker;
    std::string m_end_dialog_text;
    PortraitId m_portrait_id;
    bool m_has_shown_mid_boss_dialog;
    bool m_has_shown_end_dialog;
};

}
