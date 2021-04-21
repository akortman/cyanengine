#include <catch2/catch.hpp>
#include <iostream>

#include "cyan/src/util/string.hpp"

using namespace cyan::string_util;


TEST_CASE("string_util::ltrim_inplace", "[util][string_util]") {
    SECTION("empty string") {
        std::string s;
        ltrim_inplace(s);
        CHECK(s.empty());
    }

    SECTION("string with left spacing") {
        std::string s("    abc");
        CHECK(ltrim_inplace(s) == "abc");
        CHECK(s == "abc");
    }

    SECTION("string with right spacing") {
        std::string s("abc    ");
        CHECK(ltrim_inplace(s) == "abc    ");
        CHECK(s == "abc    ");
    }

    SECTION("string with left and right spacing") {
        std::string s("      abc    ");
        CHECK(ltrim_inplace(s) == "abc    ");
        CHECK(s == "abc    ");
    }

    SECTION("string with only internal spacing") {
        std::string s("abc hhh    test");
        CHECK(ltrim_inplace(s) == "abc hhh    test");
        CHECK(s == "abc hhh    test");
    }

    SECTION("string with left, right, and internal spacing") {
        std::string s("    abc hhh    test   ");
        CHECK(ltrim_inplace(s) == "abc hhh    test   ");
        CHECK(s == "abc hhh    test   ");
    }
}

TEST_CASE("string_util::rtrim_inplace", "[util][string_util]") {
    SECTION("empty string") {
        std::string s;
        rtrim_inplace(s);
        CHECK(s.empty());
    }

    SECTION("string with left spacing") {
        std::string s("    abc");
        CHECK(rtrim_inplace(s) == "    abc");
        CHECK(s == "    abc");
    }

    SECTION("string with right spacing") {
        std::string s("abc    ");
        CHECK(rtrim_inplace(s) == "abc");
        CHECK(s == "abc");
    }

    SECTION("string with left and right spacing") {
        std::string s("      abc    ");
        CHECK(rtrim_inplace(s) == "      abc");
        CHECK(s == "      abc");
    }

    SECTION("string with only internal spacing") {
        std::string s("abc hhh    test");
        CHECK(rtrim_inplace(s) == "abc hhh    test");
        CHECK(s == "abc hhh    test");
    }

    SECTION("string with left, right, and internal spacing") {
        std::string s("    abc hhh    test   ");
        CHECK(rtrim_inplace(s) == "    abc hhh    test");
        CHECK(s == "    abc hhh    test");
    }

}

TEST_CASE("string_util::trim_inplace", "[util][string_util]") {
    SECTION("empty string") {
        std::string s;
        trim_inplace(s);
        CHECK(s.empty());
    }

    SECTION("string with left spacing") {
        std::string s("    abc");
        CHECK(trim_inplace(s) == "abc");
        CHECK(s == "abc");
    }

    SECTION("string with right spacing") {
        std::string s("abc    ");
        CHECK(trim_inplace(s) == "abc");
        CHECK(s == "abc");
    }

    SECTION("string with left and right spacing") {
        std::string s("      abc    ");
        CHECK(trim_inplace(s) == "abc");
        CHECK(s == "abc");
    }

    SECTION("string with only internal spacing") {
        std::string s("abc hhh    test");
        CHECK(trim_inplace(s) == "abc hhh    test");
        CHECK(s == "abc hhh    test");
    }

    SECTION("string with left, right, and internal spacing") {
        std::string s("    abc hhh    test   ");
        CHECK(trim_inplace(s) == "abc hhh    test");
        CHECK(s == "abc hhh    test");
    }

}

TEST_CASE("string_util::trim", "[util][string_util]") {
    SECTION("empty string") {
        std::string s;
        CHECK(trim(s).empty());
    }

    SECTION("string with left spacing") {
        std::string s("    abc");
        CHECK(trim(s) == "abc");
        CHECK(s == "    abc");
    }

    SECTION("string with right spacing") {
        std::string s("abc    ");
        CHECK(trim(s) == "abc");
        CHECK(s == "abc    ");
    }

    SECTION("string with left and right spacing") {
        std::string s("      abc    ");
        CHECK(trim(s) == "abc");
        CHECK(s == "      abc    ");
    }

    SECTION("string with only internal spacing") {
        std::string s("abc hhh    test");
        CHECK(trim(s) == "abc hhh    test");
        CHECK(s == "abc hhh    test");
    }

    SECTION("string with left, right, and internal spacing") {
        std::string s("    abc hhh    test   ");
        CHECK(trim(s) == "abc hhh    test");
        CHECK(s == "    abc hhh    test   ");
    }
}

TEST_CASE("string_util::startswith", "[util][string_util]") {
    CHECK(startswith("", ""));
    CHECK(startswith("abc", "a"));
    CHECK(startswith("abc", "ab"));
    CHECK(startswith("abc", "abc"));
    CHECK_FALSE(startswith("abc", "A"));
    CHECK(startswith(" abc", " a"));
    CHECK_FALSE(startswith(" abc", "a"));
    CHECK_FALSE(startswith("abc", " a"));
    CHECK_FALSE(startswith("abc", "abcdef"));
}

TEST_CASE("string_util::endswith", "[util][string_util]") {
    CHECK(endswith("", ""));
    CHECK(endswith("abc", "c"));
    CHECK(endswith("abc", "bc"));
    CHECK(endswith("abc", "abc"));
    CHECK_FALSE(endswith("abc", "C"));
    CHECK(endswith("abc ", "c "));
    CHECK_FALSE(endswith("abc ", "c"));
    CHECK_FALSE(endswith("abc", "c "));
    CHECK_FALSE(endswith("abc", "zabc"));
}

TEST_CASE("string_util::replace_all_inplace", "[util][string_util]") {
    SECTION("All empty") {
        std::string s;
        replace_all_inplace(s, "", "");
        CHECK(s.empty());
    }

    SECTION("Empty replacement and str") {
        std::string s;
        replace_all_inplace(s, "x", "");
        CHECK(s.empty());
    }

    SECTION("Empty str") {
        std::string s;
        replace_all_inplace(s, "x", "y");
        CHECK(s.empty());
    }

    SECTION("Empty replacement string") {
        std::string s("x");
        replace_all_inplace(s, "x", "");
        CHECK(s.empty());
    }

    SECTION("Single-char replacement of entire string") {
        std::string s("x");
        replace_all_inplace(s, "x", "y");
        CHECK(s == "y");
    }

    SECTION("Single-char replacement of middle segment") {
        std::string s("a b c");
        replace_all_inplace(s, "b", "x");
        CHECK(s == "a x c");
    }

    SECTION("Complex usage") {
        std::string s("the quick brown fox jumped over the lazy dog");
        replace_all_inplace(s, "quick", "slow");
        replace_all_inplace(s, "lazy", "sleeping");
        CHECK(s == "the slow brown fox jumped over the sleeping dog");
        replace_all_inplace(s, " ", "_");
        CHECK(s == "the_slow_brown_fox_jumped_over_the_sleeping_dog");
    }
}

TEST_CASE("string_util::split", "[util][string_util]") {
    std::vector<std::string> v;

    SECTION("Empty string") {
        split("", v);
        CHECK(v.empty());
    }

    SECTION("one token") {
        split("hhh", v);
        REQUIRE(v.size() == 1);
        CHECK(v[0] == "hhh");
    }

    SECTION("all delimiters") {
        split("        ", v);
        CHECK(v.empty());
    }

    SECTION("two token") {
        split("ab cd", v);
        REQUIRE(v.size() == 2);
        CHECK(v[0] == "ab");
        CHECK(v[1] == "cd");
    }

    SECTION("complex string") {
        split("    abcdef hhhhhh     test     ", v);
        REQUIRE(v.size() == 3);
        CHECK(v[0] == "abcdef");
        CHECK(v[1] == "hhhhhh");
        CHECK(v[2] == "test");
    }

    v.clear();
}

TEST_CASE("string_util::join (container argument)", "[util][string_util]") {
    SECTION("joining multiple values") {
        std::vector<std::string> v{"a", "b", "c"};
        CHECK(join(v) == "abc");
        CHECK(join(v, "_") == "a_b_c");
    }

    SECTION("empty vector") {
        std::vector<std::string> v;
        CHECK(join(v).empty());
        CHECK(join(v, "_").empty());
    }
}

TEST_CASE("string_util::join (begin/end iterator argument)", "[util][string_util]") {
    SECTION("joining multiple values") {
        std::vector<std::string> v{"a", "b", "c"};
        CHECK(join(v.begin(), v.end()) == "abc");
        CHECK(join(v.begin(), v.end(), "_") == "a_b_c");
    }

    SECTION("empty vector") {
        std::vector<std::string> v;
        CHECK(join(v.begin(), v.end()).empty());
        CHECK(join(v.begin(), v.end(), "_").empty());
    }

}