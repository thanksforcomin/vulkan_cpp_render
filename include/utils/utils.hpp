#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace utils {
    template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    };
}