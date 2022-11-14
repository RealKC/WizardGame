#pragma

#include "Collider.h"
#include "Bullet.h"
#include "Entity.h"

namespace WizardGame {

class Player : public Entity {
public:
    Player(Collider);
    ~Player() override;

    Bullet make_bullet() const;

    virtual void render(SDL_Renderer*);
private:
};

}
