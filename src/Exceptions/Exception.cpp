#include "Exception.h"

#include <cstring>
#include <sstream>

namespace WizardGame {

std::string Exception::format_error_message(char const* category, char const* message, char const* user_message, char const* error)
{
    std::stringstream stream;

    stream << category << ": " << message;

    if (std::strlen(user_message) != 0) {
        stream << ": " << user_message;
    }

    stream << " with error " << error;

    return stream.str();
}

}
