#pragma once

#include "../GenericTypes.h"
#include "../Size.h"
#include "../Vec2.h"
#include "Collider.h"

namespace WizardGame {

class Bullet {
public:
    static Bullet radial(Vec2 starting_position, Size size, int distance, float angle, float speed);
    static Bullet liniar(Vec2 starting_position, Size, Direction direction, float speed);

    ~Bullet();

    HasHitWall move();

    Vec2 position() const;
    Size size() const;
    Collider collider() const { return m_collider; }

private:
    Bullet(Collider, float speed);

    Collider m_collider;
    Vec2 m_origin;
    int m_distance;
    float m_angle;
    float m_speed;
};

}