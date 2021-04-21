#pragma once

#include <string>
#include <unordered_map>

#include "cyan/src/logging/logger.hpp"
#include "cyan/src/logging/error.hpp"
#include "resource.hpp"


namespace cyan {
    /**
     * ModuleManager maintains assignments from module names to module locations on the filesystem.
     */
    struct ModuleManager {
        /**
         * Get the filesystem location of a module from a module name.
         * @param module_name The name of the module
         * @return The location of the module on the filesystem.
         */
        [[nodiscard]] std::string get_module_location(const std::string& module_name) const;

        /**
         * Add a module to the manager
         * @param module_name The name assigned to the module
         * @param module_location The location of the module
         */
        void add_module(const std::string& module_name, const std::string& module_location);

        /**
         * Given a resource name (which includes module and relative path), return the absolute path to the resource.
         * @param res The name of the resource (as a ResourceName) .e.g. ResourceName{"module::resources/resource.ext"}
         * @return The absolute path to the resource
         */
        [[nodiscard]] std::string resolve_resource_path(const ResourceName& res) const;

    private:
        /**
         * Transform a provided path into an absolute path to prevent errors from paths with identical location
         * that differ in insignificant ways (.e.g. "C:/modules/mod/"
         * @param path
         * @return
         */
        static std::string make_absolute_path(const std::string& path);

        std::unordered_map<std::string, std::string> modules;
    };
}