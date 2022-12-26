#pragma once

#include "SDLObjectError.h"
#include <SDL2/SDL_ttf.h>

namespace WizardGame {

class TTFObjectError final : public SDLObjectError {
public:
    TTFObjectError(char const* category, char const* font_name, int ptsize)
        : SDLObjectError(category, FailureTo::Load, format_name_and_size(font_name, ptsize).c_str(), TTF_GetError())
    {
    }
private:
    static std::string format_name_and_size(char const* font_name, int ptsize);
};

}
