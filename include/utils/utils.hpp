#pragma once

#include <functional>
#include <string>
#include <fstream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace utils {
    template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    };

    std::string read_from_file(std::string filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if(!file.is_open())
            throw std::runtime_error("failed to open file " + filepath);

        size_t file_size = (size_t)file.tellg(); 
        std::string result;
        result.reserve(file_size);

        file.seekg(0); //make sure we're reading from the beginning
        file.read(&result[0], file_size); //read data from file

        file.close();
        return result;
    }  
}