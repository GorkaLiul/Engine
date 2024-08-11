#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Window{
public: 
    Window(int width, int height, const char* title);
    ~Window();
private: 
    int width;
    int height;
    const char* title;
public:
    GLFWwindow* window;
};

void resize_viewport_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
