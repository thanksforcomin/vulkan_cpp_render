#include <include/utils/utils.hpp>

namespace utils {
    std::vector<char> read_from_file(std::string filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if(!file.is_open())
            throw std::runtime_error("failed to open file " + filepath);

        size_t file_size = (size_t)file.tellg(); 
        std::vector<char> result(file_size);
        //result.reserve(file_size);

        file.seekg(0); //make sure we're reading from the beginning
        file.read(&result[0], file_size); //read data from file

        file.close();

        std::cout << "DEBUF: \n SIZE: " << file_size << "\n FINAL DATA SIZE << " << result.size() << "\n";
        
        return result;
    };
}