#pragma once

#include "Exception.h"
#include <SDL2/SDL.h>

namespace WizardGame {

enum class FailureTo {
    Load,
    Create,
};

class SDLObjectError : public Exception {
public:
    SDLObjectError(char const* category, FailureTo type) : SDLObjectError(category, type, "", SDL_GetError())
    {
    }

private:
    SDLObjectError(char const* category, FailureTo type, char const* user_message, char const* error)
        : Exception(format_error_message(category, type == FailureTo::Load ? "Failure to load" : "Failure to create", user_message, error))
    {
    }
};

} // WizardGame
