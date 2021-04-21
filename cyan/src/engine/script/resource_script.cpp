#include "resource_script.hpp"


using namespace cyan;
using namespace cyan::resource;


void cyan::chai_add_resource_library(cyan::ChaiEngine& chai_engine, ResourceManager& resource_manager)
{
    auto& chai = chai_engine.get_chai_object();
    chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());

    // Use the X_RESOURCES x-macro to generate a get_*_resource for each resource type.
    #define X(ResourceT) \
    using GetResourceFnPtrT = Resource<ResourceT> (ResourceManager::*)(const std::string&); \
    m->add(chaiscript::fun( \
                static_cast<GetResourceFnPtrT>(&ResourceManager::get_resource<ResourceT>), &resource_manager), \
        "get_" + string_util::to_snake_case(#ResourceT) + "_resource");
    X_RESOURCES
    #undef X

    chai.add(m);
}

