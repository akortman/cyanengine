#include <catch2/catch.hpp>

#include "cyan/src/engine/resource/resource_manager.hpp"

using namespace cyan;

TEST_CASE("ResourceManager", "[engine][resource]") {
    ResourceManager resource_manager;
    resource_manager.module_manager().add_module(
            "example_module",
            "../cyan/test/engine/resource/example_module");

    // First get_resource() call will load the resource.
    CHECK_NOTHROW(
            resource_manager.get_resource<resource::Script>(
                    ResourceName("example_module::script/script.chai"))
    );

    // Second get_resource<> call should access the loaded resource.
    CHECK_NOTHROW(
            resource_manager.get_resource<resource::Script>(
                    ResourceName("example_module::script/script.chai"))
    );

    CHECK(resource_manager.get_resource<resource::Script>(ResourceName("example_module::script/script.chai"))
            ->source == "print(\"Hello, world!\");");
}
