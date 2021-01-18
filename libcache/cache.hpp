#ifndef _SW_CACHE_H
#define _SW_CACHE_H

#include <cstddef> // size_t
#include <unordered_map>

namespace caches {

template<typename Key, typename T>
class cache {

    public:

    T get(const Key &k) {
        auto it = entry_storage.find(k);
        if (it == entry_storage.end()) {
            throw std::out_of_range("key not found error");
        }

        touch(k);
        return it->second;
    }


    void put(const Key &k, const T &val) {

        auto it = entry_storage.find(k);
        if (it != entry_storage.end()) {
            // update val for exisitng key
            it->second = val;
            touch(k);
            return;
        }
        
        if (size() >= cache::capacity()) {
            evict();
        }

        entry_storage.insert(std::make_pair(k, val));
        policy_put(k, val);
    }


    void resize(std::size_t new_capacity) {
        size_t cap_diff = _capacity - new_capacity;

        // eviction takes place when down-sizing
        for (int i = 0; i < cap_diff; ++i) {
            evict();
        }
        _capacity = new_capacity;
    }


    std::size_t capacity() {
        return _capacity;
    }


    std::size_t size() {
        return entry_storage.size();
    }


    protected:
    
    std::unordered_map<Key, T> entry_storage;
    std::size_t _capacity;

    cache(std::size_t capacity_in) : _capacity(capacity_in) {};

    virtual void policy_put(const Key &k, const T &val) = 0;
    virtual void touch(const Key &k) = 0;
    virtual void evict() = 0;
};
} // namespace caches

#endif