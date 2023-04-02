#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <future>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <filesystem>

namespace engine {
    void compile_shader(std::string filepath) {
#ifdef _WIN32
        throw std::runtime_error("fuck шindows");
#endif
#ifdef __linux__
        pid_t ret;
        std::string shader_name = filepath.substr(0, filepath.find_last_of(".")) + ".spv";

        switch(ret=fork()) {
            case -1: //error
                return;
            case 0: // derived process
                execl("/bin/glslc", "glslc", filepath.c_str(), "-o", shader_name.c_str(), NULL);
                // error checking
                // std::perror("when loading shader, error occured ");
                std::cout << "loading shader " << filepath << " is complete\n";
                break;
            default: //parent process
                return;
            }
#endif
    }

    void compile_shader_recursive(std::string directory) { //compile every file in directory
#ifdef _WIN32
        throw std::runtime_error("fuck шindows");
#endif
#ifdef __linux__
        for (const auto &item : std::filesystem::directory_iterator(directory))
                compile_shader(item.path());
#endif
    }
}