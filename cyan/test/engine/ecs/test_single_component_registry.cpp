/// We won't test this thoroughly, as SingleComponentRegistry is an intermediate between ObjectRegistry and the ECS
/// class - any problems will present themselves in one of those two classes (or also ComponentMap).
/// TODO

#include <catch2/catch.hpp>

#include "cyan/src/engine/ecs/single_component_registry.hpp"

using namespace cyan;

TEST_CASE("SingleComponentRegistry<T>", "[engine][ecs]") {
    SingleComponentRegistry<int> registry;

    CHECK(registry.size() == 0);

    SECTION("Test creation and removal via component ID") {
        auto entry = registry.add(Entity{0}, 0);
        CHECK(registry.size() == 1);
        CHECK(entry.entity.id == 0);
        CHECK(entry.id.id == 0);
        CHECK(bool(entry));
        CHECK(*entry == 0);

        registry.remove(entry.id);
        CHECK(registry.size() == 0);
        CHECK_FALSE(bool(registry.get(entry.id)));
    }

    SECTION("Test creation and removal via entity ID") {
        auto entry = registry.add(Entity{1}, 2);
        CHECK(registry.size() == 1);
        CHECK(entry.entity.id == 1);
        CHECK(entry.id.id == 0);
        CHECK(bool(entry));
        CHECK(*entry == 2);

        registry.remove(Entity{1});
        CHECK(registry.size() == 0);
        CHECK_FALSE(bool(registry.get(entry.id)));
    }

    /// TODO: more is needed.
}

