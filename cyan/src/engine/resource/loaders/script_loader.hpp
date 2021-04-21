#pragma once

#include "resource_loader.hpp"
#include "cyan/generated/resources/Script.hpp"
#include "cyan/src/util/string.hpp"


namespace cyan {
    /**
     * ResourceLoader to load resource::Script resources.
     */
    struct ScriptResourceLoader: public ResourceLoader<resource::Script> {
        Resource<resource::Script> load_resource(const std::string& path) override {
            try {
                return std::make_shared<resource::Script>(
                        string_util::file_contents(path));
            } catch (cyan::Error& err) {
                throw cyan::ResourceLoadError(err);
            }
        }
    };
}