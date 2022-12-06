#pragma once

#include <string>

namespace WizardGame {

struct Vec2 {
    int x;
    int y;

    std::string to_string() const;
};

Vec2 operator/(Vec2 lhs, int rhs);
Vec2 operator+(Vec2 lhs, Vec2 rhs);
Vec2 operator-(Vec2 lhs, Vec2 rhs);
bool operator==(Vec2 lhs, Vec2 rhs);

}
