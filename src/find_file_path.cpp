#include "find_file_path.h"
#include <filesystem>
namespace fs = std::filesystem;

std::string find_file_path( std::string name) {
    for (auto const& dir_entry : fs::recursive_directory_iterator(".."))
        if ( auto const fname = dir_entry.path().string(); fname.find(name) != std::string::npos)
            return fname.c_str();
    throw("Couldnt find path for file named: " + name);
}