#include <catch2/catch.hpp>

#include "cyan/src/engine/ecs/ecs_common.hpp"

using namespace cyan;
using namespace cyan::ecs_impl;

TEST_CASE("get_generation", "[engine][ecs]") {
    CHECK(get_generation(0) == 0);
    CHECK(get_generation(1) == 0);
    CHECK(get_generation(0xFFFFFFFFFFFFFFFF) == 0xFFFFFFFF);
    CHECK(get_generation(0x00000000FFFFFFFF) == 0x0);
    CHECK(get_generation(0xFFFFFFFF00000000) == 0xFFFFFFFF);
    CHECK(get_generation(0xFFFFFFF0F0000000) == 0xFFFFFFF0);
}

TEST_CASE("get_index", "[engine][ecs]") {
    CHECK(get_index(0) == 0);
    CHECK(get_index(1) == 1);
    CHECK(get_index(0xFFFFFFFFFFFFFFFF) == 0xFFFFFFFF);
    CHECK(get_index(0x00000000FFFFFFFF) == 0xFFFFFFFF);
    CHECK(get_index(0xFFFFFFFF00000000) == 0x0);
    CHECK(get_index(0xFFFFFFF0F0000000) == 0xF0000000);
}

TEST_CASE("make_ecs_id", "[engine][ecs]") {
    CHECK(make_ecs_id(0, 0) == 0);
    CHECK(make_ecs_id(0, 1) == 1);
    CHECK(make_ecs_id(0, 0xFFFFFFFF) == 0xFFFFFFFF);
    CHECK(make_ecs_id(0xFFFFFFFF, 0x0) == 0xFFFFFFFF00000000);
    CHECK(make_ecs_id(0xFFFFFFFF, 0xFFFFFFFF) == 0xFFFFFFFFFFFFFFFF);
}