#pragma once

#include "ecs_common.hpp"

#include <vector>

namespace cyan {
    struct Entity {
        EcsIdT id = ECS_NULL_INDEX;
    };
}

namespace std
{
    template<> struct hash<cyan::Entity>
    {
        std::size_t operator()(const cyan::Entity& e) const noexcept
        {
            return e.id;
        }
    };
}