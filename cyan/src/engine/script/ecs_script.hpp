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
    inline void chai_add_ecs_library(
            cyan::ChaiEngine& chai_engine,
            ECS& ecs_object = cyan::ecs::global_ecs) {
        // TODO: Ideally this would all be in an "ecs" namespace. However, that doesn't seem to want to compile.
        auto& chai = chai_engine.get_chai_object();
        chai.add(chaiscript::user_type<Entity>(), "Entity");
        chai.add(chaiscript::fun(&ECS::new_entity, &ecs_object), "new_entity");
        chai.add(chaiscript::fun(&ECS::delete_entity, &ecs_object), "delete_entity");
        chai.add(chaiscript::fun(&ECS::exists, &ecs_object), "entity_exists");

        // TODO: Component operations. This is a difficult problem, because we need to add each component type
        //       individually - which isn't ideal. Will probably build a X-macro of component types to make this work
        //       smoothly, along with perhaps an autogeneration script for component types?
    }
}