#pragma once

#include "../../Utils.h"
#include "../Enemy.h"

namespace WizardGame {
namespace Enemies {

class Adrian : public Enemy {
public:
    Adrian(Collider collider, Vec2 target_position, std::vector<Attack> attacks, int health, int score, std::string const& dialog)
        : Enemy(collider, target_position, attacks)
        , m_max_health(health)
        , m_health(health)
        , m_score(score)
    , m_dialog(dialog)
    {
    }

    float health_percentage() const { return (float)m_health / m_max_health; }

    bool has_pending_dialog() const;
    std::string const& dialog_text() const { return m_dialog; }

    // ^Enemy
    virtual void tick(std::vector<Bullet>& bullets, uint32_t current_time) override;
    virtual Died hit() override;

    // ^Entity
    virtual void render(SDL_Renderer*, SpriteManager&) override;
    virtual std::int64_t score_value() const override { return m_score * m_health / (100 + m_score); }

protected:
    // ^Entity
    virtual SpriteId sprite_id() const override { return SpriteId::Adrian; }

private:
    int m_max_health;
    int m_health;
    int m_score;

    std::string m_dialog;
};

} // WizardGame
} // Enemies
