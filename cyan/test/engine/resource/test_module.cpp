#include <catch2/catch.hpp>

#include "cyan/src/engine/resource/module.hpp"

using namespace cyan;

TEST_CASE("ModuleManager", "[engine][resource]") {
    ModuleManager mm;

    mm.add_module("the_module", "C:/cyanengine/modules/the_module");
    CHECK(mm.get_module_location("the_module") == "C:/cyanengine/modules/the_module");

    // Create a resource name and check that resource path resolution works.
    ResourceName rn("the_module::scripts/scr.chai");
    CHECK(mm.resolve_resource_path(rn) == "C:/cyanengine/modules/the_module/scripts/scr.chai");

    // Create another module and check that everything still works.
    mm.add_module("dlc", "C:/cyanengine/modules/dlc_module");
    CHECK(mm.get_module_location("the_module") == "C:/cyanengine/modules/the_module");
    CHECK(mm.get_module_location("dlc") == "C:/cyanengine/modules/dlc_module");

    ResourceName rn_dlc("dlc::images/grass.png");
    CHECK(mm.resolve_resource_path(rn_dlc) == "C:/cyanengine/modules/dlc_module/images/grass.png");

    // Check that we can create an already existing module with identical path without issue, but redefining with a
    // different path throws an error.
    CHECK_NOTHROW(mm.add_module("dlc", "C:/cyanengine/modules/dlc_module"));
    CHECK_THROWS(mm.add_module("dlc", "C:/cyanengine/dlc"));

    // Check that getting a nonexistent module throws an error.
    CHECK_THROWS(mm.get_module_location("no_module"));
}