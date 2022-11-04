#pragma once

#include "Vec2.h"

namespace WizardGame {

class Entity {
public:
    virtual ~Entity();

    Vec2 position() const { return m_position; }

    void move_by(int delta_x, int delta_y);

protected:
    Entity(Vec2 starting_position);
    
private:
    Vec2 m_position;
};

}
