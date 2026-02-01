#pragma once

#include <string>
#include <stdexcept>

#define exception(msg) \
    throw std::runtime_error(std::string("ERROR: ") + (msg) + \
                             "\n  File: " + __FILE__ + \
                             "\n  Line: " + std::to_string(__LINE__))
                             