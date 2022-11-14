#pragma once

#include "Collider.h"
#include "Size.h"
#include "Vec2.h"

namespace WizardGame {

class Entity {
public:
    virtual ~Entity();

    Vec2 position() const { return Vec2 { m_collider.x(), m_collider.y() }; }

    void move_by(int delta_x, int delta_y);

protected:
    Entity(Collider);

    Size size() const { return Size { m_collider.w(), m_collider.h() }; }

private:
    Collider m_collider;
};

}
