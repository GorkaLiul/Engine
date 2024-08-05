#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

Shader::Shader(const char* path_to_vertex_shader, const char* path_to_fragment_shader) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShader;
    std::ifstream fShader;

    const char* vShaderCode;  
    const char* fShaderCode; 
    // Ensure ifstream objects can throw exceptions:
    vShader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShader.open(path_to_vertex_shader);
        fShader.open(path_to_fragment_shader);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShader.rdbuf();
        fShaderStream << fShader.rdbuf();

        vShader.close();
        fShader.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        vShaderCode = vertexCode.c_str();
        fShaderCode = fragmentCode.c_str();

    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::COULD_NOT_READ_FILE_SUCCESSFULLY\n";
    }
    unsigned int vertexShader, fragmentShader;

    // Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){
  glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

