#include<iostream>
#include "window.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

Window::Window(int width, int height, const char* title) : width(width), height(height), title(title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, title, NULL, NULL); 
    glfwMakeContextCurrent(window); //tells glfw to use this window in the openGl context

    if(window == NULL){
        std::cerr<<"failed to create Window!";
        glfwTerminate();
    }

    //import & check gl functions    
    gladLoadGL();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}
}

Window::~Window(){
    glfwTerminate();
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
        glfwTerminate();
    }
}

void resize_viewport_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width, height);
}
