#include <libcache/lru.hpp> // relative path used in Makefile
#include <libcache/lfu.hpp> // relative path used in Makefile
#include <iostream>
#include <string>

// cache supports both custom types key and value
// operator== and hash function are required for custom key
struct Employee {
    std::string name;
    int age, id;

    bool operator==(const Employee &other) const{
        return this->id == other.id;
    }
};

struct HashEmployee {
    size_t operator()(const Employee &emp) const{
        return std::hash<int>{} (emp.id);
    }
};

int main() {

    // supports get, put, resize
    caches::lfu<int, int> cache(4);
    std::cout << "cache size: " << cache.size() << std::endl;
    std::cout << "cache capacity: " << cache.capacity() << std::endl;

    cache.put(3, 33);
    cache.put(2, 22);
    cache.put(5, 55);

    cache.put(3, 333);
    std::cout << cache.get(3) << ", " << cache.get(5) << std::endl;
    // 333, 55

    cache.resize(2);
    try {
        // '2' is the LFU, should've been evicted due to resize
        cache.get(2);
    }
    catch (std::out_of_range &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "--------------\n";

    // supports custom (user-defined) type
    const std::size_t CACHE_SIZE = 256;
    caches::lru<Employee, std::string, HashEmployee> access_cache(CACHE_SIZE);

    Employee john = {"John", 25, 40923127};
    Employee jane = {"Jane", 32, 40149212};

    access_cache.put(jane, "8:05");
    access_cache.put(john, "8:13");
    
    std::cout << "Access times: " 
              << jane.name << "- " << access_cache.get(jane) << ", "
              << john.name << "- " << access_cache.get(john) 
              << std::endl;
    // Access times: Jane- 8:05, John- 8:13

    return 0;
}