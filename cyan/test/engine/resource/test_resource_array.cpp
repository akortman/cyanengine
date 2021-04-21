#include <catch2/catch.hpp>

#include "cyan/src/engine/resource/resource_array.hpp"

using namespace cyan;

TEST_CASE("ResourceArray<int>", "[engine][resource]") {
    // A simple resource loader that just transforms the resource "path" into an integer.
    struct TestIntResourceLoader: public ResourceLoader<int> {
        Resource<int> load_resource(const std::string& path) override {
            return std::make_shared<int>(std::stoi(path));
        };
    };

    ResourceArray<int> resource_array(
            std::make_unique<TestIntResourceLoader>(TestIntResourceLoader())
    );

    auto res0 = resource_array.get_resource("0");
    CHECK(*res0 == 0);
    CHECK(resource_array.get_resource("0") == res0);

    auto res1 = resource_array.get_resource("1");
    CHECK(*res0 == 0);
    CHECK(*res1 == 1);
    CHECK(resource_array.get_resource("1") == res1);
    CHECK(resource_array.get_resource("0") != resource_array.get_resource("1"));
    CHECK(res0 != res1);
}