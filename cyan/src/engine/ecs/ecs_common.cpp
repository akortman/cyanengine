#include "ecs_common.hpp"

#include "cyan/src/logging/assert.hpp"

using namespace cyan;


EcsGenerationT ecs_impl::get_generation(EcsIdT id)
{
    return id >> ECS_INDEX_BITS;
}


EcsIndexT ecs_impl::get_index(EcsIdT id)
{
    return id & ECS_INDEX_MASK;
}


EcsIdT ecs_impl::make_ecs_id(EcsGenerationT generation, EcsIndexT index)
{
    CYAN_ASSERT(index == (index & ECS_INDEX_MASK));
    return index | (generation << ECS_INDEX_BITS);
}
