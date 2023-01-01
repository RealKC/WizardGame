#pragma once

#include <stdint.h>

namespace WizardGame {
namespace Enemies {

struct Attack {
    enum class Type {
        Circle,
        Line,
        ThreeAtOnce,
        Pentagram,
    };

    Type type;
    int cooldown;
    uint32_t last_fired_at;
};

}
}
