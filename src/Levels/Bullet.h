#pragma once

#include "../GenericTypes.h"
#include "../Size.h"
#include "../SpriteManager.h"
#include "../Vec2.h"
#include "Collider.h"

namespace WizardGame {

class Bullet {
public:
    static Bullet radial(Vec2 starting_position, Size size, int distance, float angle, float speed, SpriteId);
    static Bullet liniar(Vec2 starting_position, Size, Direction direction, float speed, SpriteId);

    ~Bullet();

    HasHitWall move();

    Vec2 position() const;
    Size size() const;
    Collider collider() const { return m_collider; }

    void render(SpriteManager&);

private:
    Bullet(Collider, float speed, SpriteId);

    Collider m_collider;
    Vec2 m_origin;
    SpriteId m_sprite_id;
    int m_distance;
    float m_angle;
    float m_speed;
};

}
