#pragma once

#include "Collider.h"
#include "Size.h"
#include "Vec2.h"

namespace WizardGame {

enum class Direction {
    Left,
    Right,
    Up,
    Down,
};

class Bullet {
public:
    static Bullet radial(Vec2 starting_position, Size size, int distance, float angle);
    static Bullet liniar(Vec2 starting_position, Size, Direction direction);

    ~Bullet();

    void move();

    Vec2 position() const;
    Size size() const;

private:
    Bullet(Collider);

    Collider m_collider;
    int m_distance;
    float m_angle;
};

}
