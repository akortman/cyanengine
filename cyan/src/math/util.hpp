#pragma once

#include <algorithm>

// Remove some old windows macro for max() somewhere in the below includes (otherwise a compilation error will occur)
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace cyan {
    /// Clip a value x within min and max values.
    /// Returns x if minv <= x <= maxv, returns minv if x < minv, and returns maxv if x > maxv.
    template <typename T>
    T clip(T x, T minv, T maxv) {
        return std::max(std::min(x, maxv), minv);
    }
}