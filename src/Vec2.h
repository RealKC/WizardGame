#pragma once

#include <string>
#include <strstream>

namespace WizardGame {

struct Vec2 {
    int x;
    int y;

    std::string to_string() const {
        std::strstream str;
        str << "{ " << x << ", " << y << " }";
        return str.str();
    }
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

inline bool operator==(Vec2 lhs, Vec2 rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

}
