/// TODO

#include <catch2/catch.hpp>

#include "cyan/src/engine/ecs/ecs.hpp"

using namespace cyan;

TEST_CASE("ECS: Entity creation, removal, and recreation", "[engine][ecs]") {
    ECS ecs;

    // Create some entities and ensure they have the expected IDs and we can access them all.
    Entity e0 = ecs.new_entity();
    CHECK(e0.id == 0);
    Entity e1 = ecs.new_entity();
    CHECK(e1.id == 1);
    Entity e2 = ecs.new_entity();
    CHECK(e2.id == 2);
    CHECK(ecs.exists(e0));
    CHECK(ecs.exists(e1));
    CHECK(ecs.exists(e2));

    // Now delete an entity.
    ecs.delete_entity(e1);
    CHECK(ecs.exists(e0));
    CHECK_FALSE(ecs.exists(e1));
    CHECK(ecs.exists(e2));

    // Now we create a new entity.
    Entity e1_1 = ecs.new_entity();
    CHECK(e1_1.id != e1.id);
    CHECK(ecs.exists(e0));
    CHECK_FALSE(ecs.exists(e1));
    CHECK(ecs.exists(e1_1));
    CHECK(ecs.exists(e2));

    // Delete the remaining entities.
    ecs.delete_entity(e0);
    ecs.delete_entity(e1_1);
    ecs.delete_entity(e2);
    CHECK_FALSE(ecs.exists(e0));
    CHECK_FALSE(ecs.exists(e1));
    CHECK_FALSE(ecs.exists(e1_1));
    CHECK_FALSE(ecs.exists(e2));
}

TEST_CASE("ECS: Single entity and component manipulation", "[engine][ecs]") {
    ECS ecs;
    ecs.register_component_type<int>("int");

    Entity e = ecs.new_entity();
    auto int_component_entry = ecs.add_component<int>(e, 3);

    CHECK(ecs.exists(e));
    CHECK(ecs.has_component<int>(e));
    CHECK(ecs.component_exists<int>(int_component_entry.id));
    REQUIRE(bool(ecs.get_component<int>(e)));
    CHECK(*ecs.get_component<int>(e) == 3);
    CHECK_FALSE(ecs.has_component<unsigned char>(e));

    // Attempt to add a new component. This should fail (and log a warning), not modifying the component.
    auto int_component_entry_replaced = ecs.add_component<int>(e, 4);
    CHECK(ecs.has_component<int>(e));
    CHECK(int_component_entry.id.id == int_component_entry_replaced.id.id);
    REQUIRE(bool(ecs.get_component<int>(e)));
    CHECK(*ecs.get_component<int>(e) == 3);

    // Remove the component and re-check.
    ecs.remove_component<int>(e);
    CHECK_FALSE(ecs.has_component<int>(e));
    CHECK_FALSE(bool(ecs.get_component<int>(e)));
    CHECK_FALSE(ecs.component_exists<int>(int_component_entry.id));
    CHECK(ecs.get_component<int>(e).value == nullptr);

    // Now, we add another integer component and check that it's successfully been added, then delete the entity itself
    // (without explicitly deleting the integer component) and make sure we can't access the component anymore.
    ecs.add_component(e, 5);
    CHECK(ecs.has_component<int>(e));
    REQUIRE(bool(ecs.get_component<int>(e)));
    CHECK(*ecs.get_component<int>(e) == 5);

    CHECK(ecs.exists(e));
    ecs.delete_entity(e);
    CHECK_FALSE(ecs.exists(e));
    CHECK_FALSE(ecs.has_component<int>(e));
    CHECK(ecs.get_component<int>(e).value == nullptr);
}

TEST_CASE("ECS: Multi-entity, multi-component usage", "[engine][ecs]") {
    ECS ecs;
    ecs.register_component_type<int>("int");
    ecs.register_component_type<std::string>("string");
    ecs.register_component_type<unsigned char>("byte");

    Entity e0 = ecs.new_entity();
    Entity e1 = ecs.new_entity();
    Entity e2 = ecs.new_entity();

    auto e0_int_component_entry = ecs.add_component<int>(e0, 0);
    auto e1_str_component_entry = ecs.add_component<std::string>(e1, "1");
    auto e2_int_component_entry = ecs.add_component<int>(e2, 2);
    auto e2_str_component_entry = ecs.add_component<std::string>(e2, "2");

    CHECK(ecs.exists(e0));
    CHECK(ecs.exists(e1));
    CHECK(ecs.exists(e2));
    CHECK(ecs.has_component<int>(e0));
    CHECK(ecs.has_component<std::string>(e1));
    CHECK(ecs.has_component<int>(e2));
    CHECK(ecs.has_component<std::string>(e2));
    CHECK(*ecs.get_component<int>(e0) == 0);
    CHECK(*ecs.get_component<std::string>(e1) == "1");
    CHECK(*ecs.get_component<int>(e2) == 2);
    CHECK(*ecs.get_component<std::string>(e2) == "2");

    // Test modifications.
    *ecs.get_component<int>(e0) += 10;
    *ecs.get_component<std::string>(e2) = "hello, world!";

    // Test for consistency.
    CHECK(*ecs.get_component<int>(e0) == 10);
    CHECK(*ecs.get_component<std::string>(e1) == "1");
    CHECK(*ecs.get_component<int>(e2) == 2);
    CHECK(*ecs.get_component<std::string>(e2) == "hello, world!");

    // Delete a component and test for consistency.
    ecs.remove_component<int>(e0);
    CHECK_FALSE(ecs.has_component<int>(e0));
    CHECK(*ecs.get_component<std::string>(e1) == "1");
    CHECK(*ecs.get_component<int>(e2) == 2);
    CHECK(*ecs.get_component<std::string>(e2) == "hello, world!");

    // Delete an entity and test for consistency.
    ecs.delete_entity(e2);
    CHECK_FALSE(ecs.has_component<int>(e0));
    CHECK(*ecs.get_component<std::string>(e1) == "1");
    CHECK_FALSE(ecs.exists(e2));
    CHECK_FALSE(ecs.has_component<int>(e2));
    CHECK_FALSE(ecs.has_component<std::string>(e2));
    CHECK_FALSE(ecs.has_component<unsigned char>(e2));
    CHECK_FALSE(ecs.component_exists<int>(e2_int_component_entry.id));
    CHECK_FALSE(ecs.component_exists<std::string>(e2_str_component_entry.id));

    // Add a new entity.
    Entity e3 = ecs.new_entity();
    ecs.add_component<unsigned char>(e3, 'h');
    CHECK_FALSE(ecs.has_component<int>(e0));
    CHECK(*ecs.get_component<std::string>(e1) == "1");
    CHECK_FALSE(ecs.exists(e2));
    CHECK_FALSE(ecs.has_component<int>(e2));
    CHECK_FALSE(ecs.has_component<std::string>(e2));
    CHECK_FALSE(ecs.has_component<unsigned char>(e2));
    CHECK_FALSE(ecs.component_exists<int>(e2_int_component_entry.id));
    CHECK_FALSE(ecs.component_exists<std::string>(e2_str_component_entry.id));
    CHECK(ecs.has_component<unsigned char>(e3));
    CHECK(*ecs.get_component<unsigned char>(e3) == 'h');
}

TEST_CASE("ECS: Fuzzy mass usage tests", "[engine][ecs]") {
    // TODO
}