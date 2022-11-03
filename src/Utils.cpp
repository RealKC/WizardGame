#include "Utils.h"

#include <iostream>

namespace WizardGame {

std::ostream& error()
{
    return std::cerr << "[ERROR] ";
}

std::ostream& info()
{
    return std::cerr << "[INFO] ";
}

}
