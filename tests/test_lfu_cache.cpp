#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <libcache/lfu.hpp> // relative path used in Makefile

#include <string>

// g++ -std=c++14 -I ../ test_lfu_cache.cpp -o test_lfu.out

TEST_CASE("LFU cache standard operations", "[cache::lfu]") {

    const int CACHE_SIZE = 4;
    lfu<std::string, int> c(CACHE_SIZE);

    REQUIRE(c.size() == 0);
    REQUIRE(c.capacity() == CACHE_SIZE);

    SECTION("LFU cache is within capacity") {
        c.put("A", 1);
        c.put("B", 2);
        c.put("C", 3);
        c.put("D", 4);
        REQUIRE(c.size() == CACHE_SIZE);

        // verify the existence and value
        REQUIRE(c.get("A") == 1);
        REQUIRE(c.get("B") == 2);
        REQUIRE(c.get("C") == 3);
        REQUIRE(c.get("D") == 4);
    }

    SECTION("LFU cache is full, eviction is performed") {
        // making frequency counts to 3 for A, C, D
        c.get("A");
        c.get("C");
        c.get("D");

        // evicting "B" because it has the lowest count: 2
        c.put("E", 5);
        REQUIRE(c.get("E") == 5);
        REQUIRE_THROWS_AS(c.get("B"), std::out_of_range);
        REQUIRE(c.size() == CACHE_SIZE);

        // scramble counts

    }

    SECTION("LFU cache updating values") {

        c.put("C", 30);
        c.put("D", 42);
        REQUIRE(c.get("C") == 30);
        REQUIRE(c.get("D") == 42);
        REQUIRE(c.size() == CACHE_SIZE);

        // current counts
        // A: 3
        // E: 2
        // C, D: 5
    }
    
    SECTION("LFU cache thrashing-ish") {
        std::string letters = "ZYXWVUTSRQ";
        c.put(letters[0], 0);
        REQUIRE_THROWS_AS(c.get("E"), std::out_of_range);

        // count for "Z" is 1
        for (int i = 1; i < 10; ++i) {
            std::string last_key(1, letters[i - 1]);
            std::string new_key(1, letters[i]);
            c.put(new_key, i);
            REQUIRE_THROWS_AS(c.get(last_key), std::out_of_range);
        }
        REQUIRE(c.size() == CACHE_SIZE);
    }
    

}

TEST_CASE("LFU cache custom operations", "[cache::lfu]") {

    // resize
    // print ?

}