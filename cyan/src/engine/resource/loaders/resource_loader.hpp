#pragma once

#include <memory>
#include <string>

#include "../module.hpp"
#include "../resource.hpp"
#include "cyan/src/logging/error.hpp"


namespace cyan {
    /// Create a specific exception for failures that occur upon an attempted resource load.
    CYAN_MAKE_ERROR_TYPE(ResourceLoadError)

    /**
     * Interface for loading resources. Resource loader classes should inherit from this and implement load_resource.
     * Should throw cyan::ResourceNotFoundError on failure to find the resource.
     * @tparam T The type of the resulting resource.
     * @param path The path to the resource file. The format of this file is dependent on the implementation of
     *             `load_resource`.
     */
    template<typename T>
    struct ResourceLoader {
        virtual Resource<T> load_resource(const std::string& path) = 0;
    };
}
