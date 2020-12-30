#include <libcache/lfu.hpp> // relative path used in Makefile
#include <iostream>

// g++ -std=c++14 -I ../ example.cpp -o example.out

int main() {

    auto cache = lfu<int, int>(5);
    std::cout << cache.size() << std::endl;
    std::cout << cache.capacity() << std::endl;
    return 0;
}