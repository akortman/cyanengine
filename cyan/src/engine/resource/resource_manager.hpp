#pragma once

#include "cyan/src/engine/garbage_collect_interface.hpp"
#include "cyan/generated/resources/resources.hpp"
#include "cyan/generated/resources/resources_x_list.hpp"
#include "resource_array.hpp"
#include "resource.hpp"
#include "loaders/all_resource_loaders.hpp"


namespace cyan {
    /**
     * ResourceManager manage loading and (shared) ownership of all loaded resources, as well as managing resource
     * modules via it's internal ModuleManager.
     */
    struct ResourceManager: public GarbageCollectedContainer {
        /**
         * Create a ResourceManager.
         */
        ResourceManager() :
            #define X(ResourceT) \
            resource_array_##ResourceT(std::make_unique<ResourceT ## ResourceLoader>()),
            X_RESOURCES
            #undef X
            _placeholder(false)
        {}

        /**
         * Get a resource T by string name (as a const reference).
         * @tparam T The type of resource to retrieve
         * @param name The name of the resource to retrieve. This is a resource name, not a path, and should have the
         *             form `{module-name}::{resource-path-within-module}
         * @return A Resource<T> resource handle
         */
        template <typename T>
        Resource<T> get_resource(const std::string& name) {
            return get_resource<T>(ResourceName(name));
        }

        /**
         * Get a resource T by string name (as an rvalue reference).
         * @tparam T The type of resource to retrieve
         * @param name The name of the resource to retrieve. This is a resource name, not a path, and should have the
         *             form `{module-name}::{resource-path-within-module}
         * @return A Resource<T> resource handle
         */
        template <typename T>
        Resource<T> get_resource(std::string&& name) {
            return get_resource<T>(ResourceName(name));
        }

        /**
         * Get a resource T by ResourceName.
         * (The generic template for this function will throw an error - template specializations are used for each T).
         * @tparam T The type of resource to retrieve
         * @param name The name of the resource to retrieve
         * @return A Resource<T> resource handle
         */
        template <typename T>
        Resource<T> get_resource(const ResourceName& name) {
            throw cyan::Error("get_resource<T>() was called, but T is not recognized as a resource type.");
        }

        /// Template specializations for get_resource.
        #define X(ResourceT) \
        template <> \
        Resource<resource::ResourceT> get_resource<resource::ResourceT>(const ResourceName& name) { \
            return resource_array_##ResourceT.get_resource(_module_manager.resolve_resource_path(name)); \
        }
        X_RESOURCES
        #undef X

        /**
         * Retrieve the underlying ModuleManager.
         */
        ModuleManager& module_manager() {
            return _module_manager;
        }

        /**
         * Run a garbage collection cycle on each resource array
         * @param generator A random_device to select random elements
         * @param iters The number of times to check for an unused resource (for each array).
         */
        void gc(std::random_device& generator, int iters) override {
            #define X(ResourceT) \
            resource_array_##ResourceT.gc(generator, iters);
            X_RESOURCES
            #undef X
        }

    private:
        ModuleManager _module_manager;

        // Use X-macros to generate a set of ResourceArrays.
        #define X(ResourceT) ResourceArray<resource::ResourceT> resource_array_##ResourceT;
        X_RESOURCES
        #undef X
        bool _placeholder;
    };
}