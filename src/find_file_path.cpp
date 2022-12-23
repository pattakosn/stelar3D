#include "find_file_path.h"
#include <filesystem>
#include <iostream>
//#include "assets_path.h"
namespace fs = std::filesystem;

std::string find_file_path( std::string name) {
    for (auto const& dir_entry : fs::recursive_directory_iterator("../../../..")){ //PROJECT_TOP_LEVEL_PATH.cstr()))
       // std::cout << "checking: " << dir_entry.path()<<std::endl;
        if ( auto const fname = dir_entry.path().string(); fname.find(name) != std::string::npos)
            return fname.c_str();
    }
    throw("Couldnt find path for file named: " + name);
}
