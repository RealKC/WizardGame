#pragma once

namespace WizardGame {

class Collider {
public:
    Collider(int x, int y, int w, int h);
    ~Collider();

    bool check_collision_with(Collider const& other) const;

    void apply_position_delta(int delta_x, int delta_y);

    int x() const { return m_x; }
    int y() const { return m_y; }
    int w() const { return m_w; }
    int h() const { return m_h; }

private:
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

}