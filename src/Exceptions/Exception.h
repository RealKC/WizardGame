#pragma once

#include <exception>
#include <string>

namespace WizardGame {

class Exception : std::exception {
public:
    virtual char const* what() const noexcept final { return m_message.c_str(); }

protected:
    Exception(std::string message)
        : m_message(message)
    {
    }

    static std::string format_error_message(char const* category, char const* message, char const* user_message, char const* error);

public:
    std::string m_message;
};

}
