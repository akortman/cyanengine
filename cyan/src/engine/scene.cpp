#include "scene.hpp"


cyan::Scene::Scene(std::string name) : _scene_name(std::move(name)) {}


const std::string& cyan::Scene::name()
{
    return _scene_name;
}


cyan::ECS& cyan::Scene::ecs()
{ return _ecs; }


cyan::systems::SystemsManager& cyan::Scene::systems()
{ return _systems; }
