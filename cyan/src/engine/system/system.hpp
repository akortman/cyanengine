#pragma once


#include <cyan/src/engine/scene.hpp>
#include "cyan/src/engine/ecs/ecs.hpp"
#include "cyan/src/math/util.hpp"

namespace cyan {
    struct Scene;
}

namespace cyan::systems {
    struct Priority {
        Priority() = delete;
        explicit Priority(int p) : value(clip(p, 0, max)) {}
        constexpr static int max = 8;
        const int value;
    };

    struct System {
        explicit System(int priority) : _priority(priority) {}
        virtual void update(Scene& scene, double ticks_elapsed) = 0;
        [[nodiscard]] int priority() const { return _priority.value; }
    private:
        Priority _priority;
    };
}