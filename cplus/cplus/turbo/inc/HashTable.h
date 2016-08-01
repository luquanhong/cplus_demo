// --------------------------------------------------------------------------------
// HashTable
//
// This uses MurmurHash3 to construct a hash table.
//
// --------------------------------------------------------------------------------
#ifndef __TURBO_HASH_TABLE_H__
#define __TURBO_HASH_TABLE_H__

#include <sys/types.h>

#include "List.h"
#include "MurmurHash3.h"

namespace turbo {

template <typename K, typename V>
class HashTable {

public:
    explicit HashTable();

public:
    void put(const K& key, const V& value);
    V& get(const K& key);

    void remove(const K& key);

    void clear();

private:
    static size_t kHashTableBucketCount = 256;
    List<V> _buckets[kHashTableBucketCount];

    typedef uint32_t hash_type;
    hash_type _getHashValue(const K& key);

private:
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);

}; // class HashTable

template <typename K, typename V>
HashTable::clear()
{
    for (size_t i = 0; i < kHashTableBucketCount; i++)
        _buckets[i].clear();
}

template <typename K, typename V>
HashTable::remove(const K& key)
{
}

template <const char*, typename V>
HashTable::hash_type HashTable::_getHashValue(const const char*& key)
} // namespace turbo

#endif // __TURBO_HASH_TABLE_H__
