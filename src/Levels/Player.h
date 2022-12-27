#pragma once

#include "Bullet.h"
#include "Collider.h"
#include "Entity.h"

namespace WizardGame {

enum class LostFinalLife {
    Yes,
    No,
};

class Player : public Entity {
    static constexpr unsigned INITIAL_LIVES = 5;

public:
    Player(Collider, Size render_size);
    ~Player() override;

    Bullet make_bullet() const;

    bool has_iframes() const;
    void decrease_iframes();
    LostFinalLife die();
    void reset_lives() { m_lives = INITIAL_LIVES; }
    void go_back_to_spawn() { move_to(m_spawn_location.x, m_spawn_location.y); }

    // ^Entity
    virtual void render(SDL_Renderer*, SpriteManager&);

private:
    Vec2 m_spawn_location;
    Size m_render_size;
    unsigned m_lives;
    unsigned m_iframes;
};

}
