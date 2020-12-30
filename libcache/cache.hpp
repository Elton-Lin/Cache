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

    size_t size() {
        return m_size;
    }
    
    size_t capacity() {
        return m_capacity;
    }

    protected:
    std::size_t m_size;
    std::size_t m_capacity;

    // cache() : capacity(0) {};
    cache(std::size_t _capacity) : m_capacity(_capacity) {};
    // ~ cahce() {}; // virtual?

};

#endif