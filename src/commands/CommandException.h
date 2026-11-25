#pragma once

#include <stdexcept>
#include <string>

class CommandException : public std::runtime_error
{
public:
    explicit CommandException(const std::string &message)
        : std::runtime_error(message)
    {
    }
};