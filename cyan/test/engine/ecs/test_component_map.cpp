/// TODO

#include <catch2/catch.hpp>
#include <string>

#include "cyan/src/engine/ecs/component_map.hpp"

using namespace cyan;
using namespace cyan::ecs_impl;

TEST_CASE("ComponentMap: unique type identifiers", "[engine][ecs]") {
    ComponentMap component_map;
    component_map.register_component_type<int>("int");
    component_map.register_component_type<std::string>("string");
    component_map.register_component_type<unsigned char>("byte");

    CHECK(component_map.get_component_name<int>() == "int");
    CHECK(component_map.get_component_name<std::string>() == "string");
    CHECK(component_map.get_component_name<unsigned char>() == "byte");

    CHECK(component_map.get_component_type_id<int>().id == 0);
    CHECK(component_map.get_component_type_id<std::string>().id == 1);
    CHECK(component_map.get_component_type_id<unsigned char>().id == 2);

    // Check twice to make sure nothing has changed (each get() call is assigning a new mapping from the type to a
    // component registry, so things may be modified accidentally).
    CHECK(component_map.get_component_type_id<int>().id == 0);
    CHECK(component_map.get_component_type_id<std::string>().id == 1);
    CHECK(component_map.get_component_type_id<unsigned char>().id == 2);
}


TEST_CASE("ComponentMap: access to underlying registries", "[engine][ecs]") {
    ComponentMap component_map;
    component_map.register_component_type<int>("int");
    component_map.register_component_type<std::string>("string");
    component_map.register_component_type<unsigned char>("byte");

    component_map.get_component_registry<int>()->add(Entity{0}, 0);
    component_map.get_component_registry<int>()->add(Entity{1}, 1);
    CHECK(*component_map.get_component_registry<int>()->get(Entity{0}) == 0);
    CHECK(*component_map.get_component_registry<int>()->get(Entity{1}) == 1);

    // Delete Entity 0's component and check that we've still got Entity 1's.
    component_map.get_component_registry<int>()->remove(Entity{0});
    CHECK_FALSE(bool(component_map.get_component_registry<int>()->get(Entity{0})));
    CHECK(bool(component_map.get_component_registry<int>()->get(Entity{1})));

    // Add a new component from entity 2.
    component_map.get_component_registry<int>()->add(Entity{2}, 2);
    CHECK_FALSE(bool(component_map.get_component_registry<int>()->get(Entity{0})));
    CHECK(bool(component_map.get_component_registry<int>()->get(Entity{1})));
    CHECK(bool(component_map.get_component_registry<int>()->get(Entity{2})));
    CHECK(*component_map.get_component_registry<int>()->get(Entity{1}) == 1);
    CHECK(*component_map.get_component_registry<int>()->get(Entity{2}) == 2);

    // Now add string components and check that they work as expected.
    component_map.get_component_registry<std::string>()->add(Entity{0}, "0");
    component_map.get_component_registry<std::string>()->add(Entity{1}, "1");
    component_map.get_component_registry<std::string>()->add(Entity{2}, "2");
    CHECK(*component_map.get_component_registry<std::string>()->get(Entity{0}) == "0");
    CHECK(*component_map.get_component_registry<std::string>()->get(Entity{1}) == "1");
    CHECK(*component_map.get_component_registry<std::string>()->get(Entity{2}) == "2");

    // Check that our integer components are still as expected.
    CHECK(*component_map.get_component_registry<int>()->get(Entity{1}) == 1);
    CHECK(*component_map.get_component_registry<int>()->get(Entity{2}) == 2);

    // Remove all integer components, then check that the string components are as expected.
    component_map.get_component_registry<int>()->remove(Entity{1});
    component_map.get_component_registry<int>()->remove(Entity{2});
    CHECK_FALSE(bool(component_map.get_component_registry<int>()->get(Entity{0})));
    CHECK_FALSE(bool(component_map.get_component_registry<int>()->get(Entity{1})));
    CHECK_FALSE(bool(component_map.get_component_registry<int>()->get(Entity{2})));
    REQUIRE(bool(component_map.get_component_registry<std::string>()->get(Entity{0})));
    REQUIRE(bool(component_map.get_component_registry<std::string>()->get(Entity{1})));
    REQUIRE(bool(component_map.get_component_registry<std::string>()->get(Entity{2})));
    REQUIRE_FALSE(bool(component_map.get_component_registry<std::string>()->get(Entity{3})));
    CHECK(*component_map.get_component_registry<std::string>()->get(Entity{0}) == "0");
    CHECK(*component_map.get_component_registry<std::string>()->get(Entity{1}) == "1");
    CHECK(*component_map.get_component_registry<std::string>()->get(Entity{2}) == "2");
}
