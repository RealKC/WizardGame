#pragma

#include "Collider.h"
#include "Entity.h"

namespace WizardGame {

class Player : public Entity {
public:
    Player(Collider);
    ~Player() override;

private:
};

}
