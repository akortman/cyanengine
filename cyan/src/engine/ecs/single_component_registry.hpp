#pragma once

#include "entity.hpp"
#include "cyan/src/logging/assert.hpp"
#include "cyan/src/engine/ecs/object_registry.hpp"
#include "cyan/src/logging/logger.hpp"

#include <queue>
#include <unordered_map>
#include <vector>

namespace cyan {
    /** SingleComponentRegistry
     * Provides a wrapper over an ObjectRegistry with the addition of an entity-to-component ID mapping (and also in
     * reverse).
     * @tparam T The type of the component to wrap.
     */
    template <typename T>
    struct SingleComponentRegistry {
        /// Handle on an ID for cleaner code.
        struct Id { EcsIndexT id = ECS_NULL_INDEX; };

        /**
         * ComponentArray<T>::Entry objects are used to refer to component values.
         * These act as a pointer to the value.
         * Should **not** be stored as a handle to the component (use an ID for that).
         */
        struct Entry {
            Entity entity;
            Id id;
            T* value;

            /// operator* overloads to allow this object to be used as if it's a pointer.
            T& operator*() { return *value; }
            const T& operator*() const { return *value; }

            /// Boolean conversion to check if the entry is valid.
            explicit operator bool() { return value != nullptr; }

        private:
            /// Constructor to move an underlying ObjectRegistry entry into this entry.
            static Entry make_from_object_registry_entry(Entity e, typename ecs_impl::ObjectRegistry<T>::Entry entry)
            {
                return Entry{e, Id{entry.id}, entry.value};
            }

            friend SingleComponentRegistry<T>;
        };

        /**
         * Set the component type name, used for debugging purposes.
         * @param name The name to be assigned to the component type.
         */
        void set_component_type_name(const std::string& name) {
            this->component_type_name = name;
        }

        /**
         * Get the name of the component. Useful for debugging purposes.
         * @return A std::string of the name that has been assigned to this component type.
         */
        std::string get_component_type_name() {
            return component_type_name;
        }

        /**
         * Add a copy of an object to the array and return a reference entry.
         * @param object The object to make a copy of.
         * @return An entry which can be used to retrieve the value.
         */
        Entry add(Entity e, const T& object) {
            auto entity_component_iter = entity_component_map.find(e.id);
            if (entity_component_iter != entity_component_map.end()) {
                LOG(WARN, "Attempted to add a component of type \"{}\" to entity {}, but that component "
                          "already has an existing component of this type. The provided component will be discarded.",
                    component_type_name, e.id);
                // We return the existing component, disregarding the provided component.
                // TODO: There's an argument that adding a component to an Entity that already has a component of that
                //       type should throw an error, and this may be a good idea in non-release builds. This is a
                //       non-fatal error though, so in release we want the engine to keep running wherever possible
                //       rather than failing outright.
                return get(Id{entity_component_iter->second});
            }

            auto component_entry = components.add(object);

            entity_component_map.insert({e.id, component_entry.id});
            component_entity_map.insert({component_entry.id, e.id});

            return Entry::make_from_object_registry_entry(e, component_entry);
        }

        /**
         * Create an object in-place and return a reference entry.
         * @tparam Args The types to construct the object from.
         * @param args The values to construct the object from.
         * @return An entry which can be used to retrieve the value.
         */
        template <typename ...Args>
        Entry emplace(Entity e, Args... args) {
            auto entity_component_iter = entity_component_map.find(e.id);
            if (entity_component_iter != entity_component_map.end()) {
                LOG(WARN, "Attempted to emplace a component of type \"{}\" to entity {}, but that component "
                          "already has an existing component of this type. The provided component will be discarded.",
                    component_type_name, e.id);
                // We return the existing component.
                return get(Id{entity_component_iter->second});
            }

            auto component_entry = components.emplace(std::forward(args...));

            entity_component_map.insert({e.id, component_entry.id});
            component_entity_map.insert({component_entry.id, e.id});

            return Entry::make_from_object_registry_entry(e, component_entry);
        }

        /**
         * Look up an entry from an Entity ID.
         * @param id The id of the object.
         * @return An entry corresponding with the component associated with the provided entity
         */
        Entry get(Entity e) {
            auto entity_component_iter = entity_component_map.find(e.id);
            if (entity_component_iter == entity_component_map.end()) return make_null_entry();
            auto id = entity_component_iter->second;
            return get(Id{id});
        }

        /**
         * Look up an entry from an Component ID.
         * @param id The id of the object.
         * @return An entry for the matching component for the provided ID
         */
        Entry get(Id id) {
            auto component_entry = components.get(id.id);
            if (!component_entry) return make_null_entry();
            return Entry::make_from_object_registry_entry(Entity{component_entity_map[id.id]}, component_entry);
        }

        /**
         * Remove an object from the array.
         * @param id The id of the object to remove.
         */
        void remove(Id id) {
            auto component_entity_iter = component_entity_map.find(id.id);
            if (component_entity_iter == component_entity_map.end()) return;
            auto entity_id = component_entity_iter->second;
            entity_component_map.erase(entity_id);
            component_entity_map.erase(component_entity_iter);
            components.remove(id.id);
        }

        /**
         * Remove an object from the array via the ID of it's owning entity.
         * @param e The entity id from which to remove the component.
         */
        void remove(Entity e) {
            auto entity_component_iter = entity_component_map.find(e.id);
            // TODO: since remove(Id) looks up in component_entity_map to clear the values there, this is inefficient,
            // TODO: so it should be checked with a profiler to double-check the multiple lookups aren't a problem.
            if (entity_component_iter == entity_component_map.end()) return;
            remove(Id{entity_component_iter->second});
        }

        /**
         * Get the number of currently active elements in the registry.
         */
        [[nodiscard]]
        std::size_t size() const {
            return components.size();
        }

        /**
         * Utility function to make a null entry.
         */
        static Entry make_null_entry() {
            return Entry{Entity{ECS_NULL_INDEX}, Id{ECS_NULL_INDEX}, nullptr};
        }

    private:
        std::unordered_map<EcsIdT, EcsIdT> entity_component_map;
        std::unordered_map<EcsIdT, EcsIdT> component_entity_map;
        ecs_impl::ObjectRegistry<T> components;
        // We store the name of the component type here.
        // This is redundant, because it's also stored in ComponentMap. However, it's very useful to have it accessible
        // from within SingleComponentRegistry (so the assigned component name can be quoted on error logs) and also in
        // ComponentMap for use in a debugger (because ComponentMap uses void* to store these Registries, it's not easy
        // to debug.
        // TODO: We can likely clean up this system when we move to making SingleComponentRegistry<T> inherit from a
        //       non-templated abstract class.
        std::string component_type_name;
    };
}