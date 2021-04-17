#include <catch2/catch.hpp>

#include "cyan/src/engine/script/chai_engine.hpp"

using namespace cyan;

/// Add one to a value. For chai testing purposes.
int add_one(int i) {
    return i + 1;
}

TEST_CASE("ChaiEngine: script execution via eval(text)", "[engine][script]") {
    // TODO: When we get everything nicely wrapped by ChaiEngine, reduce usage of the internal chaiscript variable.
    ChaiEngine chai_engine;
    auto& chai = chai_engine.get_chai_object();

    chai.add(chaiscript::fun(&add_one), "add_one");

    CHECK(chai.eval<int>("add_one(0)") == 1);
    CHECK(chai.eval<int>("add_one(2)") == 3);
    CHECK(chai.eval<int>("add_one(-2)") == -1);
}

