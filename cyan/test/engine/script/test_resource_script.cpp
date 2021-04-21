#include <catch2/catch.hpp>

#include "cyan/src/engine/resource/resource_manager.hpp"
#include "cyan/src/engine/script/chai_engine.hpp"
#include "cyan/src/engine/script/resource_script.hpp"
#include "cyan/src/engine/script/generated_script.hpp"

using namespace cyan;


TEST_CASE("Resource chai script integration", "[engine][resource][script]") {
    ResourceManager resource_manager;
    ChaiEngine chai_engine;
    chai_add_resource_library(chai_engine, resource_manager);
    chai_add_codegen_generated_library(chai_engine);

    resource_manager.module_manager().add_module(
            "example_module",
            "../cyan/test/engine/resource/example_module");

    CHECK(chai_engine.run("get_script_resource(\"example_module::script/script.chai\");").ok);
    CHECK(
        chai_engine.get_chai_object().eval<std::string>(
            R"(get_script_resource("example_module::script/script.chai").source)")
        == "print(\"Hello, world!\");"
    );
}


