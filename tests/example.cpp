#include <libcache/lru.hpp> // relative path used in Makefile
#include <iostream>

// g++ -std=c++14 -I ../ example.cpp -o example.out

int main() {

    auto cache = lru<int, int>(5);
    std::cout << cache.size() << std::endl;
    std::cout << cache.capacity() << std::endl;

    cache.put(3, 1);
    cache.put(2, 8);
    cache.put(5, 7);
    cache.put(100, 12);
    cache.put(35, -111);

    cache.resize(2);

    cache.print();
    std::cout << cache.size() << std::endl;
    std::cout << cache.capacity() << std::endl;
    return 0;
}