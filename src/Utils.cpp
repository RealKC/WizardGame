#include "Utils.h"

#include <iostream>

namespace WizardGame {

std::ostream& error()
{
    return std::cerr << std::boolalpha << "[ERROR] ";
}

std::ostream& info()
{
    return std::cerr << std::boolalpha << "[INFO] ";
}

}
