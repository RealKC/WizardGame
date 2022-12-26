#pragma once

#include "Exception.h"

namespace WizardGame {

class InitError : public Exception {
public:
    InitError(char const* category, char const* error)
        : Exception(format_error_message(category, "Failed to initialise", "", error))
    {
    }
};
} // WizardGame
