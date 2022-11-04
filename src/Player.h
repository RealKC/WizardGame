#pragma

#include "Entity.h"
#include "Vec2.h"

namespace WizardGame {

class Player : public Entity {
public:
    Player(Vec2 starting_position);
    ~Player() override;

private:
};

}
