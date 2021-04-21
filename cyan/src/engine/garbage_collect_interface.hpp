#pragma once

#include <random>


namespace cyan {
/**
 * An interface for structs that have a garbage collection routine to clean up loose data.
 */
    struct GarbageCollectedContainer {
        /**
         * Run a garbage collection cycle on the object.
         * @param generator A std::random device to select elements within the structure
         * @param iters An indicator of the scale of the gc cycle (how many iterations of checking an element will be
         *              done on the container).
         */
        virtual void gc(std::random_device& generator, int iters) = 0;
    };
}
