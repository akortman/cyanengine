#pragma once

#include "cyan/src/engine/ecs/ecs.hpp"
#include "cyan/src/engine/ecs/ecs_global.hpp"
#include "chai_engine.hpp"

namespace cyan {
    /**
     * Register core ECS functions into the chai script engine.
     * @param chai_engine The ChaiEngine script engine object to add ECS functions to
     * @param ecs_object The ECS object for the functions to operate on. Defaults to the global ECS object.
     */
    extern void chai_add_ecs_library(
            cyan::ChaiEngine& chai_engine,
            ECS& ecs_object = cyan::ecs::global_ecs);
}