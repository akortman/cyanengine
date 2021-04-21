#pragma once

#include <stdexcept>
#include <fmt/format.h>

namespace cyan {
    /**
     * Internal engine errors should throw this exception (or an inheriting exception).
     */
    struct Error: public std::runtime_error {
        /// Construct a nomad::Error
        explicit Error(const std::string& message)
                : std::runtime_error(message)
        {}

        /// Create a nomad::Error with the provided format
        /// Uses fmt-style argument formatting
        /// e.g.
        ///     int value = 10;
        ///     Error("Value out of range: {}", value)
        /// will show error string "Value out of range: 10"
        template<typename... Params>
        explicit Error(Params... args)
                : std::runtime_error(fmt::format(args...))
        {}
    };
}

/**
 * Utility macro to create a new exception class inheriting from cyan::Error.
 */
#ifndef CYAN_MAKE_ERROR_TYPE
#define CYAN_MAKE_ERROR_TYPE(ERR_NAME) \
struct ERR_NAME: public cyan::Error { \
    explicit ERR_NAME(const std::string& message) : cyan::Error("(" #ERR_NAME ")" + message) {} \
    explicit ERR_NAME(const cyan::Error& err) : cyan::Error(std::string("(" #ERR_NAME ") ") + err.what()) {} \
    template<typename... Params> \
    explicit ERR_NAME(Params... args) : cyan::Error(fmt::format(args...)) {} \
};
#endif
