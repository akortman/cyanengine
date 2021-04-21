#pragma once

#include <memory>
#include <string>


namespace cyan {
    /**
     * Resource is a simple type alias for a shared_ptr<T>, used as a handle to the resource.
     */
    template<typename T>
    using Resource = std::shared_ptr<T>;

    /**
     * ResourceName is used to refer to the location of a resource on the filesystem.
     * Resources are specified by names of the form `{module-name}::{resource_path}`,
     * e.g. `cyan_core::scripts/script.chai` refers to `scripts/script.chai` in the `cyan_core` module.
     * The module name can be resolved to another filesystem location using ModuleManager, which can be used to obtain
     * the full path to the resource file.
     */
    struct ResourceName {
        /// Default construct a ResourceName with no name.
        ResourceName();
        /// Construct a ResourceName from a copied string.
        explicit ResourceName(std::string name);

        /// Get the module name that contains the resource.
        [[nodiscard]] std::string module_name() const;

        /// Get the resource path within the module.
        [[nodiscard]] std::string resource_path() const;

        /// Get the full resource name.
        [[nodiscard]] const std::string& resource_name() const;

    private:
        std::string name;
    };
}