#pragma once

namespace WizardGame {

struct Vec2 {
    int x;
    int y;
};

inline Vec2 operator/(Vec2 lhs, int rhs)
{
    return Vec2 {
        lhs.x / rhs,
        lhs.y / rhs,
    };
}

inline Vec2 operator+(Vec2 lhs, Vec2 rhs)
{
    return Vec2 {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
}

inline Vec2 operator-(Vec2 lhs, Vec2 rhs)
{
    return Vec2 {
        lhs.x - rhs.x,
        lhs.y - rhs.y
    };
}

}
