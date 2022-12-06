#pragma once

#include <string>
#include <strstream>

namespace WizardGame {

enum class HasHitWall {
    Yes,
    No,
};

class Collider {
public:
    Collider(int x, int y, int w, int h);
    ~Collider();

    bool check_collision_with(Collider const& other) const;

    HasHitWall apply_position_delta(int delta_x, int delta_y);
    HasHitWall move_to(int x, int y);

    int x() const { return m_x; }
    int y() const { return m_y; }
    int w() const { return m_w; }
    int h() const { return m_h; }

    std::string to_string() const {
        std::strstream str;
        str << "[ " << x() << ", " << y() << "; " << w() << ", " << h() << " ]";
        return str.str();
    }

private:
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

}
