#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <random>

#include "cyan/src/engine/resource/loaders/resource_loader.hpp"
#include "cyan/src/engine/garbage_collect_interface.hpp"
#include "resource.hpp"


namespace cyan {
    /**
     * ResourceArray<T> holds resources of type T, along with cleanup management.
     * @tparam T The type of the resource the ResourceArray maintains.
     */
    template <typename T>
    struct ResourceArray: public GarbageCollectedContainer {
        /**
         * Create a ResourceArray<T>
         * @param resource_loader A type that implements ResourceLoader<T>, used to load in the resource.
         */
        explicit ResourceArray(std::unique_ptr<ResourceLoader<T>> resource_loader)
            : resource_loader(std::move(resource_loader)) {}

        /**
         * Get a resource via it's name.
         * @param name The name of the resource to get
         * @return A Resource<T> to the resource.
         */
        Resource<T> get_resource(const std::string& resource_path) {
            auto name_pos_iter = resource_map.find(resource_path);
            if (name_pos_iter == resource_map.end()) {
                Resource<T> res = resource_loader->load_resource(resource_path);
                resource_map.insert({resource_path, res});
                return res;
            }
            return name_pos_iter->second;
        }

        /**
         * Run a garbage collection cycle on the resource array.
         * @param generator A random_device to select random positions in the resource array
         * @param iters The number of times to check for an unused resource.
         */
        void gc(std::random_device& generator, int iters) override {
            for (int i = 0; i < iters; i += 1) {
                // Select a random resource.
                auto random_resource_iter = std::next(
                        std::begin(resource_map),
                        std::uniform_int_distribution<int>(0, resource_map.size() - 1)(generator));
                // If the resource is not in use (use_count = 1 because of the shared_ptr held by the ResourceArray),
                // remove it from the array.
                if (random_resource_iter->second.use_count() <= 1) {
                    resource_map.erase(random_resource_iter);
                }
            }
        }

    private:
        std::unordered_map<std::string, Resource<T>> resource_map;
        std::unique_ptr<ResourceLoader<T>> resource_loader;
    };
}