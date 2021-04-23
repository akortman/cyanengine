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

TEST_CASE("SingleComponentRegistry<T>: iterator", "[engine][ecs]") {
    SingleComponentRegistry<int> registry;

    for (int i = 0; i < 100; i += 1) {
        auto e = Entity{static_cast<EcsIdT>(i)};
        registry.add(e, i);
    }

    int count = 0;
    for (auto it = registry.begin(); it != registry.end(); it++) {
        CHECK(bool(*it));
        CHECK(it->entity.id == count);
        CHECK(*it->value == count);
        count += 1;
    }
    CHECK(count == 100);

    for (int i = 0; i < 50; i += 1) {
        registry.remove(Entity{static_cast<EcsIdT>(i)});
    }

    count = 0;
    for (auto & entry : registry) {
        CHECK(bool(entry));
        CHECK(entry.entity.id == count + 50);
        CHECK(*entry.value == count + 50);
        count += 1;
    }
    CHECK(count == 50);
}