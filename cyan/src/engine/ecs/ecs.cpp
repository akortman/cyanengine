#include "ecs.hpp"


cyan::Entity cyan::ECS::new_entity()
{
    return Entity{entities.add({}).id};
}


void cyan::ECS::delete_entity(cyan::Entity e)
{
    entities.remove(e.id);
}


bool cyan::ECS::exists(cyan::Entity e)
{
    return bool(entities.get(e.id));
}
