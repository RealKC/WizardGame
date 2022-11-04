#include "KeyboardState.h"

namespace WizardGame {

KeyboardState::KeyboardState()
    : m_key_state(SDL_GetKeyboardState(nullptr))
{
}

KeyboardState::~KeyboardState()
{
    // Documentatia SDL zice ca nu trebuie sa dam dealocam pointerul returnat de SDL_GetKeyboardState
    // asa ca il lasam in pace.
}

}
