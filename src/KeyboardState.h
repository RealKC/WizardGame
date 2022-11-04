#pragma once

#include <SDL2/SDL.h>

namespace WizardGame {

class KeyboardState {
public:
    KeyboardState();
    ~KeyboardState();

    bool is_key_pressed(SDL_Scancode key) const { return m_key_state[key]; }

private:
    Uint8 const* m_key_state;
};

}
