#pragma once

#include <cstdlib>

#include "cyan/src/logging/logger.hpp"
#include "cyan/src/logging/error.hpp"

/**
 * Debug builds only: if a statement is false, log an error and exit.
 * To be used to check invariant conditions in algorithms liberally.
 * Consider throwing a cyan::Error for other situations, because CYAN_ASSERT won't be present in release builds.
 * TODO: this is should be compiled out in non-debug builds
 */
#ifndef CYAN_ASSERT
    #define CYAN_ASSERT(x) do { if (!x) { \
            LOGL(CRITICAL, "Assert failure: {}", ##x); \
            throw cyan::Error("Assert failure: {}", ##x); \
        }} while (0)
#endif
