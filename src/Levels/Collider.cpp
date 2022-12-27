#include "Collider.h"

#include "../Game.h"
#include <sstream>

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
    int bottom_a = m_y + m_h;
    int top_a = m_y;

    int left_b = other.m_x;
    int right_b = other.m_x + other.m_w;
    int bottom_b = other.m_y + other.m_h;
    int top_b = other.m_y;

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

HasHitWall Collider::apply_position_delta(int delta_x, int delta_y)
{
    auto has_hit_wall = HasHitWall::No;

    int new_x = m_x + delta_x;
    int new_y = m_y + delta_y;

    if (new_x >= 0 && new_x + m_w <= Game::WINDOW_WIDTH)
        m_x += delta_x;
    else
        has_hit_wall = HasHitWall::Yes;
    if (new_y >= 0 && new_y + m_h <= Game::WINDOW_HEIGHT)
        m_y += delta_y;
    else
        has_hit_wall = HasHitWall::Yes;

    return has_hit_wall;
}

HasHitWall Collider::move_to(int x, int y)
{
    return apply_position_delta(x - m_x, y - m_y);
}

std::string Collider::to_string() const  {
    std::stringstream str;
    str << "[ " << x() << ", " << y() << "; " << w() << ", " << h() << " ]";
    return str.str();
}

}
