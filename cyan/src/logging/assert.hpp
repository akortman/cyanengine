#pragma once

#include <cstdlib>

namespace cyan {
    /**
     * Debug builds only: if a statement is false, log an error and exit.
     * Only to be used in critical failures in logic - consider throwing an internal error in other cases (this is
     * designed to be compiled out in non-debug builds).
     * @param x The statement to be evaulated.
     */
    inline void _assert(bool x) {
        // TODO: seperate into CPP
        if (!x) {
            // TODO: LOGGING
            std::exit(-1);
        }
    };
}

#define CYAN_ASSERT(x) cyan::_assert(x)