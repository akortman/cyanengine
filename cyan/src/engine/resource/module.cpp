#include "module.hpp"

#include <filesystem>

#include "cyan/src/util/string.hpp"


std::string cyan::ModuleManager::get_module_location(const std::string& module_name) const
{
    auto it = modules.find(module_name);
    if (it == modules.end()) {
        throw cyan::Error("Module `{}` not found", module_name);
    }
    return it->second;
}


void cyan::ModuleManager::add_module(const std::string& module_name, const std::string& module_location)
{
    auto canon_module_location = make_absolute_path(module_location);
    auto it = modules.find(module_name);
    if (it != modules.end()) {
        // Reduplication of an identical module is ok - it's just slightly inefficient.
        if (it->second == canon_module_location) {
            LOG(WARN, "Repeated definition of identical module with name \"{}\", location \"{}\"",
                module_name, canon_module_location);
            return;
        }
        // Redefining an existing module with a different path is likely to create some major inconsistency
        // problems and probably missing resources, so throw an error in that case.
        throw cyan::Error(R"(Redefinition of module "{}" from location "{}" to "{}")",
                          module_name, it->second, canon_module_location);
    }
    modules.insert({module_name, canon_module_location});
}


std::string cyan::ModuleManager::resolve_resource_path(const cyan::ResourceName& res) const
{
    return make_absolute_path(get_module_location(res.module_name()) + "/" + res.resource_path());
}


std::string cyan::ModuleManager::make_absolute_path(const std::string& path)
{
    auto c = std::filesystem::weakly_canonical(std::filesystem::path(path)).string();
    string_util::replace_all_inplace(c, "\\", "/");
    return c;
}
