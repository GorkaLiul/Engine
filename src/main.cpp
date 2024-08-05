#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"
#include "shader.h"
//resize : window-> viewport 

float vertices[] = {
	-0.5f,-0.5f,0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};


int main(){

	Window Win_clas(1500, 700, "arcane Engine");
  Shader shaderProgram("../shaders/vertexShader.txt", "../shaders/fragmentShader.txt");

	// creating the vertex buffer object (loads memory cpu -> GPU)
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// links VBO buffer to an gl_array_buffer (could be other buffer type)

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	GL_STATIC_DRAW: the data is set only once and used many times.
	GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	*/



	// 0. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	// 1. set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);


	//creating VAO object:
	unsigned int VAO; 
	glGenVertexArrays(1,&VAO);

	//1. bind Vertex Array Object
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

	// keeps the window open
	while (!glfwWindowShouldClose(Win_clas.window))
	{
	
		processInput(Win_clas.window);
		glUseProgram(shaderProgram.ID);
		glBindVertexArray(VAO);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES,0, 3);

		glfwSwapBuffers(Win_clas.window);
		glfwPollEvents();
	}
	glfwTerminate();
}
