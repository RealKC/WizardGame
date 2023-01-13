#pragma once

#include <SDL.h>

namespace WizardGame {

/// Clasa asta este pentru movement-ul player-ului, adica lucruri pe care vrei sa le faci cat timp
/// o tasta e apasata
class KeyboardState {
public:
    KeyboardState();
    ~KeyboardState();

    bool is_key_pressed(SDL_Scancode key) const { return m_key_state[key]; }

private:
    Uint8 const* m_key_state;
};

}
