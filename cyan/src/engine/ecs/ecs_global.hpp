/**
 * Contains global functions that operate on a global ECS object.
 */

#pragma once

#include "ecs.hpp"

namespace cyan::ecs {
    // TODO: If we have issues with this, it may be best to use a pointer that is initialized by the engine setup.
    //       for now, it has a default constructor, so we should be fine.
    ECS global_ecs;

    /**
     * Create a new entity with no associated components.
     * @return An entry pointer to the new entity.
     */
    Entity new_entity();

    /**
     * Delete an entity from the ECS system.
     * @param e The ID of the entity to remove.
     */
    void delete_entity(Entity e);

    /**
     * Test is a given entity ID exists.
     * @param e The entity to check existence of.
     * @return Whether or not the entity exists.
     */
    bool exists(Entity e);

    /**
     * Add a component to an entity.
     * @tparam T The type of the component to add.
     * @param e The entity to add the component to
     * @param component The component data
     * @return An Entry associated with the newly-added component.
     */
    template <typename T>
    ComponentEntry<T> add_component(Entity e, const T& component) {
        return global_ecs.add_component<T>(e, component);
    }

    /**
     * Add a component to an entity, constructed in-place.
     * @tparam T The type of the component to add.
     * @tparam Args The types of the arguments to construct the component.
     * @param e The entity to add the component to
     * @param args The arguments with which to construct the component.
     * @return An Entry associated with the newly-added component.
     */
    template <typename T, typename ...Args>
    ComponentEntry<T> emplace_component(Entity e, Args... args) {
        return global_ecs.emplace_component<T>(e, std::forward(args...));
    }

    /**
     * Get a component entry for a component of type T associated with an entity with the given ID.
     * If the entity does not exist OR if the entity does not have a component of type T, a null entry will be
     * returned (note that Entry objects can be used like pointers - test for boolean value to check if it is
     * valid).
     * @tparam T The type of component to get.
     * @param e The entity from which to get the associated component.
     * @return An entry associated with the requested component, or null if no entity or component can be found.
     */
    template <typename T>
    ComponentEntry<T> get_component(Entity e) {
        return global_ecs.get_component<T>(e);
    }

    /**
     * Get a component entry for a component of type T given a component ID.
     * This function checks to see if the associated Entity still exists. If it does not, the requested component is
     * deemed invalid, will be deleted, and a null entry will be returned.
     * If the component does not exist, a null entry will be returned (note that Entry objects can be used like
     * pointers - test for boolean value to check if it is
     * valid).
     * @tparam T The type of component to get.
     * @param cid The ID of the requested component.
     * @return An entry associated with the requested component, or null if no entity or component can be found.
     */
    template <typename T>
    ComponentEntry<T> get_component(ComponentId<T> cid) {
        return global_ecs.get_component<T>(cid);
    }

    /**
     * Remove a component from an entity, given the component type and the entity ID.
     * If the entity does not have an associated component of type T, nothing will happen.
     * @tparam T The type of the component to remove.
     * @param e The entity to remove the component from.
     */
    template <typename T>
    void remove_component(Entity e) {
        global_ecs.remove_component<T>(e);
    }

    /**
     * Remove a component from an entity, given the component type and component Id.
     * @tparam T The type of the component to remove.
     * @param e The component id of the component to remove.
     */
    template <typename T>
    void remove_component(ComponentId<T> cid) {
        global_ecs.remove_component<T>(cid);
    }
}