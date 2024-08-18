#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_toggle.h"
#include <deque> // For storing frame times
#include <vector> // For plotting

class ui {
public:
    ui(GLFWwindow* window);
    void newFrame();
    void mainGui();
    void destroy();
private:
    void plotFrameRateGraph(); // Method to plot the frame rate graph
    void topBarmenu();
    void overlay();
    void plotOverlay();
    GLFWwindow* window;
    bool editor();

    std::deque<float> frameTimes; // To store frame times
    float maxHistoryTime; // Max history time in seconds
    float frameTimeSum;   // Sum of frame times for calculating average
    float timeAccumulator; // Accumulator for tracking elapsed time
    float deltaTime;       // Time difference between frames
    
    int x, y;
public:
    bool wireframe;

    //bool render;
};


