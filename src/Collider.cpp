#include "Collider.h"

namespace WizardGame {

Collider::Collider(int x, int y, int w, int h)
    : m_x(x)
    , m_y(y)
    , m_w(w)
    , m_h(h)
{
}

Collider::~Collider()
{
}

bool Collider::check_collision_with(Collider const& other) const
{
    // Implementam "AABB Collision"
    // Un articol folositor pentru a intelege conceptul: https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php

    int left_a = m_x;
    int right_a = m_x + m_w;
    int bottom_a = m_y;
    int top_a = m_y + m_h;

    int left_b = other.m_x;
    int right_b = other.m_x + other.m_w;
    int bottom_b = other.m_y;
    int top_b = other.m_y + other.m_h;

    if (bottom_a <= top_b) {
        // Obiectul A se afla sub Obiectul B
        return false;
    }

    if (top_a >= bottom_b) {
        // Obiectul A se afla deasupra Obiectului B
        return false;
    }

    if (right_a <= left_b) {
        // Obiectul A se afla la stanga Obiectului B
        return false;
    }

    if (left_a >= right_b) {
        // Obiectul A se afla la dreapta Obiectului B
        return false;
    }

    // Obiectele se intersecteaza => are loc o coliziune
    return true;
}

void Collider::apply_position_delta(int delta_x, int delta_y)
{
    m_x += delta_x;
    m_y += delta_y;
}

}
