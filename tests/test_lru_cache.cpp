#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <libcache/lru.hpp> // relative path used in Makefile

#include <string>
#include <iostream>

// g++ -std=c++14 -I ../ test_lru_cache.cpp -o test_lru.out

TEST_CASE("LRU cache standard operations", "[cache::lru]") {

    const int CACHE_SIZE = 4;
    lru<std::string, int> c(CACHE_SIZE);

    REQUIRE(c.size() == 0);
    REQUIRE(c.capacity() == CACHE_SIZE);

    c.put("A", 1);
    c.put("B", 2);
    c.put("C", 3);
    c.put("D", 4);

    SECTION("LRU cache is within capacity") {
        REQUIRE(c.size() == CACHE_SIZE);

        // verify the existence and value
        REQUIRE(c.get("A") == 1);
        REQUIRE(c.get("B") == 2);
        REQUIRE(c.get("C") == 3);
        REQUIRE(c.get("D") == 4);
    }

    SECTION("LRU cache is full, eviction is performed") {
        // this makes "B" the least recently used
        // even though it is accessed many times
        c.get("B");
        c.get("B");
        c.get("A");
        c.get("C");
        c.get("D");

        // evicting "B" because it is the LRU
        c.put("E", 5);
        REQUIRE(c.get("E") == 5);
        REQUIRE_THROWS_AS(c.get("B"), std::out_of_range);
        REQUIRE(c.size() == CACHE_SIZE);

        // scramble counts
    }

    SECTION("LRU cache updating values, no eviction should take place") {

        c.put("C", 30);
        c.put("D", 42);
        REQUIRE(c.get("A") == 1);
        REQUIRE(c.get("B") == 2);
        REQUIRE(c.get("C") == 30);
        REQUIRE(c.get("D") == 42);
        REQUIRE(c.size() == CACHE_SIZE);
    }

    SECTION("LRU cache updating values, order should also update") {
        // access order is
        // A, B, C, D
    
        c.put("A", 111);
        // access order should now be
        // B, C, D, A

        // evicting "B" because it is the LRU
        c.put("E", 5);
        REQUIRE_THROWS_AS(c.get("B"), std::out_of_range);
        REQUIRE(c.get("A") == 111);
    }
    
    SECTION("LRU cache thrashing-ish") {
        std::string cur_letters = "ABCD";
        std::string new_letters = "ZYXW";

        for (int i = 0; i < 4; ++i) {
            std::string lru_key(1, cur_letters[i]);
            std::string new_key(1, new_letters[i]);
            c.put(new_key, i);
            REQUIRE(c.get(new_key) == i);
            REQUIRE_THROWS_AS(c.get(lru_key), std::out_of_range);
        }
        REQUIRE(c.size() == CACHE_SIZE);
    }
    

}

TEST_CASE("LRU cache custom operations", "[cache::lru]") {

    // resize
    // print ?

}