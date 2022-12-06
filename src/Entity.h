#pragma once

#include "Collider.h"
#include "GenericTypes.h"
#include "Size.h"
#include "Vec2.h"
#include <SDL2/SDL.h>

namespace WizardGame {

class Bullet;

class Entity {
public:
    virtual ~Entity();

    Vec2 position() const { return Vec2 { m_collider.x(), m_collider.y() }; }

    void move_by(int delta_x, int delta_y);

    virtual void render(SDL_Renderer*) = 0;

    bool collides_with(Bullet const&) const;
    bool collides_with(Entity const&) const;

protected:
    Entity(Collider);

    Size size() const { return Size { m_collider.w(), m_collider.h() }; }

    Vec2 position_for_bullet(Size bullet_size, Direction) const;

    void move_to(int x, int y);

private:
    Collider m_collider;
};

}
