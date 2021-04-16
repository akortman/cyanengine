#include "ecs_global.hpp"

using namespace cyan;
using namespace cyan::ecs;


Entity new_entity()
{
    return global_ecs.new_entity();
}


void delete_entity(Entity e)
{
    return global_ecs.delete_entity(e);
}


bool exists(Entity e)
{
    return global_ecs.exists(e);
}
