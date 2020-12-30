#ifndef _SW_CACHE_H
#define _SW_CACHE_H

#include <cstddef> // size_t

template<typename Key, typename T>
class cache {

    public:
    virtual T get(Key k) = 0;
    virtual void put(Key k, T val) = 0;
    
    virtual void print() = 0;
    virtual void resize(std::size_t new_capacity) = 0;

    protected:
    std::size_t size;
    std::size_t capacity;

    cache(std::size_t _capacity) : capacity(_capacity) {};
    // ~ cahce() {}; // virtual?

};

#endif