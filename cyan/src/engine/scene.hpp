#pragma once


#include <utility>

#include "cyan/src/engine/ecs/ecs.hpp"
#include "cyan/src/engine/system/system_manager.hpp"


namespace cyan {
    struct Scene {
        explicit Scene(std::string name);

        const std::string& name();

        ECS& ecs();

        systems::SystemsManager& systems();

    private:
        std::string _scene_name;
        ECS _ecs;
        systems::SystemsManager _systems;
    };
}