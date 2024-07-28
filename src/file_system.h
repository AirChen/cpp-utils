#ifndef FILESYSTEM
#define FILESYTTEM

#include <string>
#include <iostream>
#include <filesystem>

namespace utils {
inline namespace v1 {

// directory
bool create_directory(std::string&& file_path) {
    std::error_code ec{};
    bool is_create_status = std::filesystem.create_directory(file_path, ec);
    if (!is_create_status) {
        std::cout << ec.message() << "\n";
    }
    return is_create_status;
}

bool cr

}
}

#endif