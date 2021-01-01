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

    c.put("A", 1);
    c.put("B", 2);
    c.put("C", 3);
    c.put("D", 4);

    SECTION("LFU cache is within capacity") {
        
        REQUIRE(c.size() == CACHE_SIZE);

        // verify the existence and value
        REQUIRE(c.get("A") == 1);
        REQUIRE(c.get("B") == 2);
        REQUIRE(c.get("C") == 3);
        REQUIRE(c.get("D") == 4);
    }

    SECTION("LFU cache is full, eviction is performed") {

        // making frequency counts to 2 for A, C, D
        c.get("A");
        c.get("C");
        c.get("D");

        // evicting "B" because it has the lowest count: 1
        c.put("E", 5);
        REQUIRE(c.get("E") == 5);
        REQUIRE_THROWS_AS(c.get("B"), std::out_of_range);
        REQUIRE(c.size() == CACHE_SIZE);

        // scramble counts
    }

    SECTION("LFU cache updating values, no eviction should take place") {

        c.put("C", 30);
        c.put("D", 42);
        REQUIRE(c.get("A") == 1);
        REQUIRE(c.get("B") == 2);
        REQUIRE(c.get("C") == 30);
        REQUIRE(c.get("D") == 42);
        REQUIRE(c.size() == CACHE_SIZE);
    }

    SECTION("LFU cache updating values, order should also update") {

        // freq counts are all 1 bt now
        // making freq counts all 2 except B
        c.put("A", 123);
        c.put("C", 456);
        c.put("D", 789);
        
        // evicting "B" because it is the LFU
        c.put("E", 5);
        REQUIRE_THROWS_AS(c.get("B"), std::out_of_range);
        REQUIRE(c.get("A") == 123);
    }
    
    SECTION("LFU cache thrashing-ish") {

        // making freq counts all 2 except D
        c.get("A");
        c.get("B");
        c.get("C");

        std::string letters = "ZYXWVUTSRQ";
        c.put(std::string(1, letters[0]), 0);
        REQUIRE_THROWS_AS(c.get("D"), std::out_of_range);

        // count for "Z" is 1
        for (int i = 1; i < 10; ++i) {
            std::string last_key(1, letters[i - 1]);
            std::string new_key(1, letters[i]);
            c.put(new_key, i);
            REQUIRE_THROWS_AS(c.get(last_key), std::out_of_range);
        }
        // A(2), B(2), C(2), Q(1)
        REQUIRE(c.get("Q") == 9);
        REQUIRE(c.get("A") == 1);
        REQUIRE(c.get("B") == 2);
        REQUIRE(c.get("C") == 3);
        REQUIRE(c.size() == CACHE_SIZE);
    }
    

}

TEST_CASE("LFU cache custom operations", "[cache::lfu]") {

    // resize
    // print ?

}