#pragma once

//glm / transformation related libs
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include<glad/glad.h>
#include<fstream>
#include<iostream>
#include<sstream>

class Shader{

public:
  unsigned int ID;

public: 
  Shader(const char* path_to_vertex_shader, const char* path_to_fragment_shader);

  void use();
  void setBool(const std::string& name, bool value) const;
  void setFloat(const std::string& name, float value) const ;
  void setInt(const std::string& name, int value) const;
  void setMat4(const std::string& name, const glm::mat4 &value) const;
  void checkCompileErrors(unsigned int shader, std::string type); 

};

