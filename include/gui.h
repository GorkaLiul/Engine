#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <deque> // For storing frame times
#include <vector> // For plotting

class ui {
public:
    ui(GLFWwindow* window);
    void newFrame();
    void mainGui( bool &render, bool &wireframe, float &colors);
    void destroy();
private:
    void plotFrameRateGraph(); // Method to plot the frame rate graph
    GLFWwindow* window;

    std::deque<float> frameTimes; // To store frame times
    float maxHistoryTime; // Max history time in seconds
    float frameTimeSum;   // Sum of frame times for calculating average
    float timeAccumulator; // Accumulator for tracking elapsed time
    float deltaTime;       // Time difference between frames
    
    int x, y;
    //bool render;
};


