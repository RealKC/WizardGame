#pragma

#include "Collider.h"
#include "Bullet.h"
#include "Entity.h"

namespace WizardGame {

enum class LostFinalLife {
    Yes,
    No,
};

class Player : public Entity {
public:
    Player(Collider, Size render_size);
    ~Player() override;

    Bullet make_bullet() const;

    LostFinalLife die();

    virtual void render(SDL_Renderer*);
private:
    Vec2 m_spawn_location;
    Size m_render_size;
    unsigned m_lives;
};

}
