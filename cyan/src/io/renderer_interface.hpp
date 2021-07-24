#pragma once

#include "cyan/src/engine/scene.hpp"

namespace cyan::io {
    /**
     * An interface for various renderer implementations.
     */
    struct RendererInterface {
        /// Draw the contents of the ECS onto the screen (will render according to what Entities have Render components)
        virtual void draw(Scene& scene) = 0;
        /// Get the name of the Renderer for debugging purposes.
        virtual const char* name() = 0;
    };
}