#ifndef _LFU_SW_CACHE_H
#define _LFU_SW_CACHE_H

#include "cache.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility> // std::move


namespace caches {

template<typename Key, typename T, 
        typename Hash = std::hash<Key>, 
        typename Eq = std::equal_to<Key> >
class lfu : public cache<Key, T, Hash, Eq> {

    struct KeyEntry;
    struct FreqBucket;

    using cache_t = cache<Key, T, Hash, Eq>;
    using bucket_iter_t = typename std::list<FreqBucket>::iterator;
    using entry_iter_t = typename std::list<KeyEntry>::iterator;

    public:

    lfu(std::size_t _capacity) : cache_t(_capacity) {}

    private:

    struct KeyEntry {
        Key entry_k;
        bucket_iter_t bucket_it;
    };

    struct FreqBucket {
        std::size_t freq_count;
        std::list<KeyEntry> entries;
    };

    const std::size_t INIT_COUNT = 1;

    // Doubly-linked list storing frequency buckets in increasing order
    // Each bucket stores a doubly-linked list of cache entries
    std::list<FreqBucket> lfu_queue;

    // Maps entry key to iterator (points to node in frequency bucket)
    std::unordered_map<Key, entry_iter_t, Hash, Eq> node_map;


    void policy_put(const Key &k, const T &val) override {
        if (lfu_queue.empty() || lfu_queue.begin()->freq_count != INIT_COUNT) {
            lfu_queue.emplace_front(
                FreqBucket {INIT_COUNT, std::list<KeyEntry>()}
            );
        }

        // insert the new entry to the '1' bucket and node map
        std::list<KeyEntry> &bucket_one_entries = lfu_queue.begin()->entries;
        bucket_one_entries.emplace_front(KeyEntry {k, lfu_queue.begin()});
        node_map.insert(std::make_pair(k, bucket_one_entries.begin()));
    }


    // Increment the freq count for this entry
    void touch(const Key &k) override {
        entry_iter_t entry = node_map[k];

        bucket_iter_t cur_bucket_it = entry->bucket_it;
        bucket_iter_t next_bucket_it = get_next_bucket(cur_bucket_it);
        
        std::list<KeyEntry> &cur_entries = cur_bucket_it->entries;
        std::list<KeyEntry> &next_entries = next_bucket_it->entries;

        // Repoint entry's iter to new bucket 
        // then move entry to the next bucket's entries
        entry->bucket_it = next_bucket_it;
        next_entries.splice(next_entries.begin(), cur_entries, entry);

        if (cur_entries.empty()) {
            lfu_queue.erase(cur_bucket_it);
        }
    }


    void evict() override {
        std::list<KeyEntry> &lfu_entries = lfu_queue.begin()->entries;
        Key evicted_key = lfu_entries.back().entry_k;

        cache_t::entry_storage.erase(evicted_key);
        node_map.erase(evicted_key);
        lfu_entries.pop_back();

        // remove the bucket if it's empty
        if (lfu_entries.empty()) {
            lfu_queue.pop_front();
        }
    }

    bucket_iter_t get_next_bucket(bucket_iter_t cur_it) {
        bucket_iter_t next_it = cur_it;
        ++next_it;// careful: can be end()

        std::size_t next_count = cur_it->freq_count + 1;
        if (next_it == lfu_queue.end() || next_count < next_it->freq_count) {
            // Inserts a new bucket (count + 1) right after current bucket
            FreqBucket new_bucket{next_count, std::list<KeyEntry>()};
            next_it = lfu_queue.insert(next_it, std::move(new_bucket));
        }
        
        return next_it;
    }

};
} // namespace caches

#endif