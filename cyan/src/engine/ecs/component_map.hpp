/** component_type_map.hpp
 * Provides utilities to map from component types to integers.
 *
 * Thanks to `https://gpfault.net/posts/mapping-types-to-values.txt.html`, which was invaluable in setting this system
 * up.
 */

#pragma once

#include <atomic>
#include <memory>
#include <unordered_map>

#include "single_component_registry.hpp"
#include "cyan/src/logging/logger.hpp"

namespace cyan::ecs_impl {
    struct ComponentTypeId {
        int id;
    };

    /**
     * ComponentMap provides an interface from the type of a component to it's corresponding array.
     * There's a lot of room for optimization here, but ideally this is how the interface should work.
     * It may require a shift from runtime (with void* pointers and all that mess) to compile-time, but I've tried that
     * before and it gets messy.
     */
    struct ComponentMap {
        /**
         * Get the integer ID associated with a component type.
         * @tparam ComponentT The type to get an integer ID for
         * @return A wrapper over the integer ID
         */
        template <typename ComponentT>
        ComponentTypeId get_component_type_id()
        {
            return ComponentTypeId{get_component_type_id_internal<ComponentT>()};
        }


        /**
         * Register a component type with a name.
         * @tparam ComponentT The type of component to register
         * @param component_name
         */
        template <typename ComponentT>
        void register_component_type(const std::string& component_name) {
            init_component_registry<ComponentT>(component_name);
        }

        /**
         * Return the ComponentRegistry corresponding with the given type.
         * @tparam ComponentT The type of component to get a registry for
         * @return A reference to the registry array
         */
        template <typename ComponentT>
        SingleComponentRegistry<ComponentT>* get_component_registry()
        {
            auto component_type_id = get_component_type_id_internal<ComponentT>();

            // If we can't find a registry with the given ID, initialize it and return the newly initialized registry.
            if (component_registries.size() <= component_type_id) {
                return init_component_registry<ComponentT>();
            }

            return (SingleComponentRegistry<ComponentT>*)(component_registries[component_type_id]);
        }

        /**
         * Get the internal name of a component.
         * @tparam ComponentT The type of the component
         * @return The name assigned to the component or Component{Id} if no name has been assigned.
         */
        template <typename ComponentT>
        const std::string& get_component_name() {
            // TODO: Potential for a segfault/undefined behavior here if we try get the name of a type that we haven't
            //       registered.
            return component_names[get_component_type_id_internal<ComponentT>()];
        }

    private:
        // TODO: There's a number of potential problems with this implementation:
        //     1.  All SingleComponentRegistry<T> objects should have the same size regardless of T - we can probably
        //         having a pointer here and just store the map in a chunk of bytes and cast the pointer over,
        //         but for now this will do until it becomes an obvious issue.
        //     2.  Probably the worst one: each component's registry is obfuscated by a void pointer to the allocated
        //         registry. This lets us do our tricky mapping types at runtime (sort of), but the downside is we
        //         can't delete this without somehow mapping back from the IDs associated with the type to the type
        //         itself, which isn't as easy (but might be possible). I'm unsure on the best way to solve this - it
        //         isn't an immediate issue, because the ECS and thus the component map will likely last the lifetime
        //         of the game, so we won't leak memory because of this, but it's not good.
        //         One idea: make SingleComponentArray<T> a child class of an abstract parent interface of some
        //         AbstractSingleComponentArray, which we can cast each void* back to and hopefully ensure cleanup is
        //         done in ComponentMap's destructor by an explicit polymorphic cleanup() function. This seems to
        //         interfere with the idea presented to Problem 1, though (or does it? I imagine there's a solution
        //         somewhere).
        std::vector<void*> component_registries;
        std::vector<std::string> component_names;
        static std::atomic_int component_type_id_counter;

        /**
         * Internal implementation to get the id of a component type.
         * This works via each templated version of this function having a different static id variable, thus assigning
         * a unique value to each new type.
         * @tparam ComponentT The type of component to get the ID of.
         * @return The ID associated with type ComponentT.
         */
        template <typename ComponentT>
        inline static int get_component_type_id_internal()
        {
            static const int id = component_type_id_counter++;
            return id;
        }

        /**
         * Internal function to initialize a component registry for a given type.
         * @tparam ComponentT The type of component to initialize a registry for.
         * @return A reference to the registry itself.
         */
        template <typename ComponentT>
        SingleComponentRegistry<ComponentT>* init_component_registry(const std::string& name = "") {
            auto component_type_id = get_component_type_id_internal<ComponentT>();

            if (component_registries.size() <= component_type_id) {
                // If the component registry doesn't already exist, create it, assign the provided name and return it.
                while (component_registries.size() <= component_type_id) {
                    // Make room in the vectors until we can store a value at component_type_id.
                    // This is somewhat unnecessary - a new type should inherently only be at position
                    // component_registries.size() because each new get_component_type_id_internal() increments the id
                    // counter. However, this is robust to implementation changes and guarantees that the vector is in
                    // a valid state in case something weird goes on, so we'll keep it unless it proves to be a
                    // performance issue (very doubtful).
                    component_registries.push_back(nullptr);
                    component_names.push_back("");
                }

                // Assign the registry at the location of the id to a valid registry.
                auto component_reg_ptr = new SingleComponentRegistry<ComponentT>{};
                component_reg_ptr->set_component_type_name(name);
                component_registries[component_type_id] = (void*)component_reg_ptr;
                component_names[component_type_id] = name;
                return component_reg_ptr;
            } else {
                // If the registry already exists, it's likely register_component_type() was called after a component
                // access for the given type already happened. This is ok - we'll set the component name to the new name
                // (as that is the most likely expected reason for this to happen). This could also happen when
                // register_component_type() was called with the same T for a second time - which may mean there will
                // be confusion about what name the component type should have. Because of the potential for errors
                // here, we will warn the user of the issue and inform them of correct usage.
                LOG(WARN, "Attempting to register component id {} with name \"{}\", but registry for that component "
                          "already exists. Component name will be modified to the new name (old name \"{}\"). "
                          "This will occur when `ECS::register_component_type()` is called either for a second time or "
                          "after a component registry of that name has already been accessed. Components should be "
                          "registered via `ECS::register_component_type()` before they are used.",
                    component_type_id, name, component_names[component_type_id]);

                // Only rename if the given name isn't default (there's no usefulness to the name being "" if the
                // existing name is a useful one).
                if (name != "") {
                    component_names[component_type_id] = name;
                    ((SingleComponentRegistry<ComponentT>*)component_registries[component_type_id])
                        ->set_component_type_name(name);
                }

                return (SingleComponentRegistry<ComponentT>*)component_registries[component_type_id];
            }
        }
    };
}
