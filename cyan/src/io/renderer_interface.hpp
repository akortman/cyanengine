#pragma once

#include "cyan/src/engine/ecs/ecs.hpp"

namespace cyan::io {
    /**
     * An interface for various renderer implementations.
     */
    struct RendererInterface {
        /// Draw the contents of the ECS onto the screen (will render according to what Entities have Render components)
        virtual void draw(ECS& ecs) = 0;
    };
}