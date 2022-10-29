#include "shader.h"
#include <filesystem>
namespace fs = std::filesystem;

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    ID = glCreateProgram();

    // vertex shader
    unsigned int vertex = compileShader(vertexPath, GL_VERTEX_SHADER);

    //fragment shader
    unsigned int fragment = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    // geometry shader
    unsigned int geometry;
    if(geometryPath != nullptr) {
        geometry = compileShader(geometryPath, GL_GEOMETRY_SHADER);
    }

    glLinkProgram(ID);
    checkCompileErrors(ID, true);
    // delete the shaders as they're linked into our program now and no longer necessary
    if(geometryPath != nullptr)
        glDeleteShader(geometry);
    glDeleteShader(fragment);
    glDeleteShader(vertex);
}

void Shader::checkCompileErrors(unsigned int shader, bool is_shader_program) {
    int success;
    char infoLog[1024];
    if (!is_shader_program) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR: " << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR: " << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

unsigned int Shader::compileShader(const char* filename, int shader_type) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string source;
    std::ifstream srcStream;
    // ensure ifstream objects can throw exceptions:
    srcStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        //for( std::string location {"../"}; const auto & entry : fs::recursive_directory_iterator(location)) {
        //    if(entry.m_type == fs::file_type::regular) {
        //        std::cout << entry.path() << std::endl;
        //        srcStream.open(entry+filename);
        //    }
        //}


        // open files
        srcStream.open(filename);
        std::stringstream srcStringStream;
        // read file's buffer contents into streams
        srcStringStream << srcStream.rdbuf();
        // close file handlers
        srcStream.close();
        // convert stream into string
        source = srcStringStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\t:" << std::string(filename) << " : " << e.what() << std::endl;
    }
    // 2. compile shader
    unsigned int shader;
    shader = glCreateShader(shader_type);
    const char* src_ref = source.c_str();
    glShaderSource(shader, 1, &src_ref, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, false);
    glAttachShader(ID, shader);
    return shader;
}