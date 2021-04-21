#include "resource.hpp"


cyan::ResourceName::ResourceName() : name("") {}


cyan::ResourceName::ResourceName(std::string name) : name(std::move(name)) {}


std::string cyan::ResourceName::module_name() const
{
    return name.substr(0, name.find("::"));
}


std::string cyan::ResourceName::resource_path() const
{
    return name.substr(name.find("::") + 2);
}


const std::string& cyan::ResourceName::resource_name() const
{
    return name;
}
