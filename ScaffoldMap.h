#ifndef HASHMAP_H
#define HASHMAP_H

// ***** DO NOT TURN IN THIS FILE *****
//
// This file is an implementation of HashMap that has correct visible
// behavior.  If you cannot get your HashMap class to work well enough to
// make progress on the other classes that use HashMaps, you can use this
// one temporarily, switch your attention to implementing the other
// classes, and go back to working on *your* HashMap implementation later.
//
// DO NOT TURN IN THIS FILE AS YOUR IMPLEMENTATION -- it uses unordered_map,
// which the spec forbids.

#include <string>
#include <unordered_map>

template <class T>
class HashMap {
public:
    HashMap(double = 0.75) {}
    void insert(const std::string& key, const T& value) { m_map[key] = value; }
    int size() const { return m_map.size(); }
    T* find(const std::string& key) { const auto& hm = *this; return const_cast<T*>(hm.find(key)); }
    const T* find(const std::string& key) const { auto it = m_map.find(key); return it == m_map.end() ? nullptr : &it->second; }
    T& operator[](const std::string& key) { return m_map[key]; }
private:
    std::unordered_map<std::string, T> m_map;
};

#endif // HASHMAP_H