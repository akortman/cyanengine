#pragma once

#include "cyan/src/engine/ecs/ecs.hpp"

namespace cyan::draw2d {
    /**
     * An interface for various renderer implementations.
     */
    struct RendererInterface {
        virtual void draw(ECS& ecs) = 0;
    };
}