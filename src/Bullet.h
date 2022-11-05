#pragma once

#include "Collider.h"
#include "Size.h"
#include "Vec2.h"

namespace WizardGame {

class Bullet {
public:
    static Bullet radial(Vec2 starting_position, Size size, int distance, float angle);
    static Bullet horizontal(Vec2 starting_position, Size size);
    static Bullet vertical(Vec2 starting_position, Size size);
    
    ~Bullet();

    void move();

    Vec2 position() const;
    Size size() const;

private:
    enum class Type {
        Radial,
        Horizontal,
        Vertical,
    };

    Bullet(Collider, Type);

    Type m_type;
    Collider m_collider;
    // Au valoare doar daca m_type == Type::Radial
    int m_distance;
    float m_angle;
};

}
