#pragma once

#include "cyan/src/engine/resource/resource_manager.hpp"
#include "chai_engine.hpp"

namespace cyan {
    /**
     * Register core resource-related functions into the chai script engine.
     * @param chai_engine The ChaiEngine script engine object to add ECS functions to
     * @param resource_manager The resource manager for resource script functions to operate on.
     */
    extern void chai_add_resource_library(
            cyan::ChaiEngine& chai_engine,
            ResourceManager& resource_manager);
}