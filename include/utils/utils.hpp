#pragma once

#include <functional>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace utils {
    std::vector<char> read_from_file(std::string filepath);

    template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    };
}