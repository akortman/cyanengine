#pragma once


#include <cyan/src/engine/system/system.hpp>
#include "chai_engine.hpp"
#include "cyan/src/engine/resource/resource.hpp"
#include "cyan/generated/resources/Script.hpp"


namespace cyan {
    extern void chai_add_systems_library(
            cyan::ChaiEngine& chai_engine,
            ECS& ecs_object);

    namespace systems {
        struct ScriptedSystem : public System {
            explicit ScriptedSystem(int priority, Resource<resource::Script> update):
                System(priority) {}

            void update(ECS& ecs, double ticks_elapsed) override {

            }
        };

    }
}