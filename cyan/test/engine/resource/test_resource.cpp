#include <catch2/catch.hpp>

#include "cyan/src/engine/resource/resource.hpp"

using namespace cyan;

TEST_CASE("ResourceName", "[engine][resource]") {
    SECTION("Simple module and path names") {
        ResourceName rn("a::b");
        CHECK(rn.resource_name() == "a::b");
        CHECK(rn.module_name() == "a");
        CHECK(rn.resource_path() == "b");
    }

    SECTION("Complex module and path names") {
        ResourceName rn("cyan_core::scripts/scr.chai");
        CHECK(rn.resource_name() == "cyan_core::scripts/scr.chai");
        CHECK(rn.module_name() == "cyan_core");
        CHECK(rn.resource_path() == "scripts/scr.chai");
    }
}