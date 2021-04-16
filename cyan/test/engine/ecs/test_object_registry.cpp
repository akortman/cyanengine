/// Tests for ObjectRegistry<T>
/// This class is critical to how the ECS system stores and reuses objects. As the ECS will be used at a large scale,
/// the tests for it should be thorough.
/// (Could probably do with some more)

#include <catch2/catch.hpp>
#include <iostream>
#include <random>

#include "cyan/src/engine/ecs/object_registry.hpp"

using namespace cyan;
using namespace cyan::ecs_impl;

TEST_CASE("ObjectRegistry: object creation, retrieval, and modification", "[engine][ecs]") {
    ObjectRegistry<int> registry;

    auto entry = registry.add(3);
    CHECK(entry.id == 0);
    REQUIRE(entry.value != nullptr);

    auto id = entry.id;
    CHECK(bool(registry.get(id)));
    CHECK(*registry.get(id) == 3);
    *registry.get(id) += 1;
    CHECK(*registry.get(id) == 4);
}

TEST_CASE("ObjectRegistry: multiple object usage", "[engine][ecs]") {
    ObjectRegistry<int> registry;

    for (int i = 0; i < 32; i += 1) {
        CHECK(registry.size() == i);
        auto entry = registry.add(i);
        CHECK(entry.id == i);
        REQUIRE(entry.value != nullptr);
        CHECK(*entry == i);
        CHECK(registry.size() == i + 1);
    }

    CHECK(registry.size() == 32);

    // Do some tests outside of a for-loop to sanity check.
    CHECK(*registry.get(0) == 0);
    CHECK(registry.get(0).id == 0);
    CHECK(*registry.get(1) == 1);
    CHECK(registry.get(1).id == 1);
    CHECK(*registry.get(2) == 2);
    CHECK(registry.get(2).id == 2);
    CHECK(*registry.get(3) == 3);
    CHECK(registry.get(3).id == 3);

    // Check that all 32 of our objects are as we expect.
    for (int i = 0; i < 32; i += 1) {
        CHECK(bool(registry.get(i)));
        CHECK(*registry.get(i) == i);
    }

    // Remove the item with id 8, and check that the objects are in the right state.
    registry.remove(8);
    CHECK(registry.size() == 31);

    for (int i = 0; i < 32; i += 1) {
        if (i == 8) {
            CHECK_FALSE(bool(registry.get(i)));
        } else {
            CHECK(*registry.get(i) == i);
        }
    }

    // Now add a new object. It should be at index 8.
    auto entry_replaced_8 = registry.add(1001);
    CHECK(bool(entry_replaced_8));
    REQUIRE(entry_replaced_8.value != nullptr);
    CHECK(*entry_replaced_8 == 1001);
    CHECK(get_index(entry_replaced_8.id) == 8);
    CHECK(get_generation(entry_replaced_8.id) == 1);
    CHECK(entry_replaced_8.id == make_ecs_id(1, 8));
    CHECK(registry.size() == 32);

    // Do a loop-over to check that no other objects have changed.
    for (int i = 0; i < 32; i += 1) {
        if (i == 8) {
            CHECK_FALSE(bool(registry.get(i)));
        } else {
            CHECK(*registry.get(i) == i);
        }
    }

    // Remove all 31 of our remaining original objects. Note that this will try (and silently fail) to remove the
    // object with id 8 - this is expected behavior!
    for (int i = 0; i < 32; i += 1) {
        registry.remove(i);
        CHECK_FALSE(bool(registry.get(i)));
    }

    CHECK(registry.size() == 1); // The entry at index 8 (generation 1, value 32) still exists

    // Now let's check to ensure they're all removed and none have somehow reverted.
    for (int i = 0; i < 32; i += 1) {
        CHECK_FALSE(bool(registry.get(i)));
    }

    // Remove entry_replaced_8 for a clean slate.
    registry.remove(entry_replaced_8.id);
    CHECK(registry.size() == 0);

    // Repopulate the array with another 32 objects.
    for (int i = 0; i < 32; i += 1) {
        registry.add(i);
    }

    CHECK(registry.size() == 32);

    // None of IDs 0-31 should be available - indices 0-31 will be in use, but with a new generation.
    // Let's make sure this is still the case:
    for (int i = 0; i < 32; i += 1) {
        CHECK_FALSE(bool(registry.get(i)));
    }

    // Let's see about our 32 new objects, which should all be generation 1, except for the one at index 8, which will
    // be generation 2.
    for (int i = 0; i < 32; i += 1) {
        cyan::EcsGenerationT gen = (i == 8) ? 2 : 1;
        cyan::EcsIndexT index = i;
        cyan::EcsIdT id = make_ecs_id(gen, index);

        auto entry = registry.get(id);
        CHECK(bool(entry));
        REQUIRE(entry.value != nullptr);
    }

    // Finally, let's delete everything and double check that they're all gone - both during removal, and after
    // everything has been deleted.
    for (int i = 0; i < 32; i += 1) {
        CHECK(registry.size() == 32 - i);
        cyan::EcsGenerationT gen = (i == 8) ? 2 : 1;
        cyan::EcsIdT id = make_ecs_id(gen, i);
        registry.remove(id);
        CHECK_FALSE(bool(registry.get(id)));
        CHECK(registry.size() == 32 - i - 1);
    }

    CHECK(registry.size() == 0);

    for (int i = 0; i < 32; i += 1) {
        cyan::EcsGenerationT gen = (i == 8) ? 2 : 1;
        cyan::EcsIdT id = make_ecs_id(gen, i);
        CHECK_FALSE(bool(registry.get(id)));
    }
}

TEST_CASE("ObjectRegistry: fuzzy mass use testing for consistency", "[engine][ecs]") {
    // Note that use of a vector here is pretty slow - when iteration over ObjectArrays is implemented, we can do this
    // better.

    // For the purposes of this test, all objects will have a value equal to their id.
    ObjectRegistry<EcsIdT> registry;
    int n_cycles = 32;
    int n_elements = 10000;
    int min_remove = 50;
    int max_remove = 5000;
    std::vector<EcsIdT> ids;

    // Create n_elements objects in the registry.
    for (int i = 0; i < n_elements; i += 1) {
        auto entry = registry.add(0);
        *entry = entry.id;
        ids.push_back(entry.id);
    }

    CHECK(registry.size() == n_elements);

    // Set up RNG for fuzzy test.
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution remove_count_rng(min_remove, max_remove);

    for (int cycle = 0; cycle < n_cycles; cycle += 1) {
        // Each cycle, we delete a random number of elements within the range [min_remove, max_remove), then add that
        // same number of new elements back.
        int n_remove = remove_count_rng(generator);

        // Remove n_remove elements.
        for (int i = 0; i < n_remove; i += 1) {
            // Select a random id.
            std::uniform_int_distribution remove_index_rng(0, (int)ids.size() - 1);
            int index_to_remove = remove_index_rng(generator);
            registry.remove(ids[index_to_remove]);
            ids.erase(ids.begin() + index_to_remove);
        }

        CHECK(registry.size() == n_elements - n_remove);
        CHECK(registry.size() == ids.size());

        // Add n_remove elements back.
        for (int i = 0; i < n_remove; i += 1) {
            auto entry = registry.add(0);
            *entry = entry.id;
            ids.push_back(entry.id);
        }

        CHECK(registry.size() == n_elements);
        CHECK(registry.size() == ids.size());
    }

    // Finally, let's check that we've maintained consistency.
    for (int i = 0; i < n_elements; i += 1) {
        auto entry = registry.get(ids[i]);
        CHECK(entry.id == *entry);
    }
}
