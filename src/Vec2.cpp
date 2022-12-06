#include "Vec2.h"

#include <sstream>

namespace WizardGame {

std::string Vec2::to_string() const {
    std::stringstream str;
    str << "{ " << x << ", " << y << " }";
    return str.str();
}

Vec2 operator/(Vec2 lhs, int rhs)
{
    return Vec2 {
        lhs.x / rhs,
        lhs.y / rhs,
    };
}

Vec2 operator+(Vec2 lhs, Vec2 rhs)
{
    return Vec2 {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
}

Vec2 operator-(Vec2 lhs, Vec2 rhs)
{
    return Vec2 {
        lhs.x - rhs.x,
        lhs.y - rhs.y
    };
}

bool operator==(Vec2 lhs, Vec2 rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

}
