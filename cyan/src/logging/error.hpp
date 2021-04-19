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