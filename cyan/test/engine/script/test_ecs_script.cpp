#include <catch2/catch.hpp>

#include "cyan/src/engine/script/chai_engine.hpp"
#include "cyan/src/engine/script/ecs_script.hpp"
#include "cyan/generated/components/components.hpp"

using namespace cyan;

TEST_CASE("chai ecs: entity creation and deletion via script", "[engine][script]") {
    ChaiEngine chai_engine;
    ECS ecs;
    chai_add_ecs_library(chai_engine, ecs);

    auto& chai = chai_engine.get_chai_object();

    // Create an entity in script and check that it exists natively.
    auto e0 = chai.eval<Entity>("new_entity();");
    CHECK(ecs.exists(e0));

    // Create another entity in script and check that it exists in script and natively.
    chai.eval<Entity>("var e1 = new_entity();");
    auto e1 = chai.eval<Entity>("e1");
    CHECK(ecs.exists(e1));
    CHECK(chai.eval<bool>("entity_exists(e1)"));
    CHECK(chai.eval<Entity>("e1").id == e1.id);
    CHECK(e1.id != e0.id);

    // Delete e1 and check for consistency.
    chai.eval("delete_entity(e1)");
    CHECK_FALSE(ecs.exists(e1));
    CHECK(ecs.exists(e0));
    CHECK_FALSE(chai.eval<bool>("entity_exists(e1)"));
}


TEST_CASE("chai ecs: component usage", "[engine][script]")
{
    ChaiEngine chai_engine;
    ECS ecs;
    chai_add_ecs_library(chai_engine, ecs);

    auto& chai = chai_engine.get_chai_object();

    auto e = chai.eval<Entity>("var e = new_entity();");
    auto chai_report = chai_engine.run("var c = add_debug_name_component(e, DebugNameComponent())");
    CHECK(chai_report.ok);
    CHECK(ecs.has_component<component::DebugName>(e));
    CHECK(bool(chai.eval<ComponentEntry<component::DebugName>>("get_debug_name_component(e)")));
    CHECK(chai.eval<bool>("get_debug_name_component(e).is_valid()"));
}