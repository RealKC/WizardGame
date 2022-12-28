#include "TTFObjectError.h"

#include <sstream>

namespace WizardGame {

std::string TTFObjectError::format_name_and_size(char const* font_name, int ptsize)
{
    std::stringstream stream;
    stream << "Font { " << font_name << ", " << ptsize << " }";
    return stream.str();
}

}
