#pragma once

#include "ecs_common.hpp"
#include "entity.hpp"
#include "object_registry.hpp"
#include "single_component_registry.hpp"
#include "component_map.hpp"

#include <vector>

namespace cyan {
    struct EntityPlaceholder {};
    using EntityRegistry = ecs_impl::ObjectRegistry<EntityPlaceholder>;
    template <typename T>
    using ComponentEntry = typename SingleComponentRegistry<T>::Entry;
    template <typename T>
    using ComponentId = typename SingleComponentRegistry<T>::Id;

    /**
     * An object containing an entire Entity-Component System (ECS) (well, no systems here, but entities and
     * components).
     * Entities can be freely created and destroyed, components can be created (for any type!) and associated with
     * entities, and can be removed via the associated entity or via it's own component ID.
     *
     * TODO: The system currently has no protections for overflow of the generation or index values, which are bitwise
     *       concatenated to form an ID. To overflow these, we would need to have either 2^32 reuses of an index within
     *       an entity/component object array (generation overflow) or 2^32 concurrent entities or components (index
     *       overflow) (note: that's 2^32 of the same type, not all together). This condition seems unlikely, but if for
     *       for some reason we need to and can actually handle millions of concurrent entities, this could be a problem
     *       to keep an eye on.
     */
    struct ECS {
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
         * Register a type as a component type. Th
         * @tparam T
         * @param name
         */
        template <typename T>
        void register_component_type(const std::string& name) {
            component_map.register_component_type<T>(name);
        }

        /**
         * Add a component to an entity.
         * @tparam T The type of the component to add.
         * @param e The entity to add the component to
         * @param component The component data
         * @return An Entry associated with the newly-added component.
         */
        template <typename T>
        ComponentEntry<T> add_component(Entity e, const T& component) {
            auto component_registry = component_map.get_component_registry<T>();
            return component_registry->add(e, component);
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
            auto component_registry = component_map.get_component_registry<T>();
            return component_registry->emplace(e, std::forward(args...));
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
            // Ensure the requested entity exists (it may have been deleted).
            // TODO: We could delete the component here, but I'm not doing that yet to ensure get_component calls do
            // TODO: as little work as possible (they're probably doing too much already). Still, if orphaned components
            // TODO: prove to be a problem, consider doing that here, but an explicit cleanup scheme is probably a
            // TODO: better idea, so try that first.
            if (!exists(e)) {
                return SingleComponentRegistry<T>::make_null_entry();
            }

            auto component_registry = component_map.get_component_registry<T>();
            return component_registry->get(e);
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
            auto component_registry = component_map.get_component_registry<T>();
            ComponentEntry<T> entry = component_registry->get(cid);

            // If the entity which owns this component no longer exists, the component isn't in a valid state, so
            // we delete the component and return a null entry.
            if (!exists(entry.entity)) {
                component_registry->remove(entry.id);
                return SingleComponentRegistry<T>::make_null_entry();
            }

            return entry;
        }

        /**
         * Remove a component from an entity, given the component type and the entity ID.
         * If the entity does not have an associated component of type T, nothing will happen.
         * @tparam T The type of the component to remove.
         * @param e The entity to remove the component from.
         */
        template <typename T>
        void remove_component(Entity e) {
            // TODO: Log an error on removal of component from nonexistent entity.
            auto component_registry = component_map.get_component_registry<T>();
            component_registry->remove(e);
        }

        /**
         * Test if a component with a given type and ID exists.
         * @tparam T The type of component to query.
         * @param cid The ID of the component to query.
         * @return Whether the component with the given type and ID exists.
         */
        template <typename T>
        bool component_exists(ComponentId<T> cid) {
            return bool(get_component<T>(cid));
        }

        /**
         * Test if a given entity has a component of type T.
         * @tparam T The type of component to query.
         * @param e The ID of the entity to query for the existence of a component of type T.
         * @return Whether the given entity has a component of type T.
         */
        template <typename T>
        bool has_component(Entity e) {
            return bool(get_component<T>(e));
        }

        /**
         * Get an entire component registry.
         * This function mainly exists for use in testing.
         * @tparam T The type of the registry to get
         * @return A SingleComponentRegistry<T>
         */
        template <typename T>
        SingleComponentRegistry<T>* get_component_registry() {
            return component_map.get_component_registry<T>();
        }

    private:
        EntityRegistry entities;
        ecs_impl::ComponentMap component_map;
    };
}