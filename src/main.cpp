#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
  #include "stb_image.h"

//resize : window-> viewport 
float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
};

unsigned int indices[] = {
  0, 1, 3,
  1, 2, 3
};


int main(){

	Window Win_class(1500, 700, "arcane Engine");
  Shader shaderProgram("../shaders/vertexShader.txt", "../shaders/fragmentShader.txt");

	// creating the vertex buffer object (loads memory cpu -> GPU)
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

  //vertex attibutes: 
  //vertex attibutes: 

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
	
  glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);


  //Textures:
  //Textures:
  
  int width, height, nrChannels;
  unsigned char* data = stbi_load("../resources/textures/cat.jpg", &width, &height, &nrChannels, 0);

 if(!data){
     std::cout<<"Failed to load Texture!"<<std::endl;
    return -1;
} 

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //texture wraping settings: 
    //texture wraping settings: 
  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = nrChannels == 4? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
   
  stbi_image_free(data);


	// keeps the window open
	while (!glfwWindowShouldClose(Win_class.window))
	{
		processInput(Win_class.window);
		glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();
    glActiveTexture(GL_TEXTURE0);// activating texture unit  ()
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

		glfwSwapBuffers(Win_class.window);
		glfwPollEvents();
	}

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  
  glfwDestroyWindow(Win_class.window);
	glfwTerminate();
}

