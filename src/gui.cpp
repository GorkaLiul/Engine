#include "gui.h"
#include<iostream>
#include <algorithm> // For std::max_element
#include<numeric>

ui::ui(GLFWwindow* window) : window(window), maxHistoryTime(5.0f), frameTimeSum(0.0f), timeAccumulator(0.0f), deltaTime(0.0f) {
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // Setup renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGui::StyleColorsDark();

    // Initialize frameTimes deque with size for smooth rendering
    frameTimes.resize(static_cast<size_t>(maxHistoryTime * 60), 0.0f); // Assuming 60 FPS for initial size

    // Set graph line color to yellow
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    ImGui::GetStyle().Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow color
}

void ui::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ui::mainGui(bool &render, bool &wireframe, float &colors) {
  glfwGetWindowSize(window, &x, &y);
    ImGui::SetNextWindowSize(ImVec2(400, y)); // Set window height to 200 pixels and full width
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set window position to the top-left corner
    ImGui::Begin("Engine", nullptr);
    ImGui::Checkbox("Render: ", &render);
    ImGui::Checkbox("WireFrame", &wireframe);
    ImGui::ColorEdit4("Color: ", &colors);
    ImGui::ShowDemoWindow();

  ImGui::SameLine();
    
    ImVec2 contentSize = ImGui::GetWindowSize();

    ImGui::End();
    ImGui::SetNextWindowSize(ImVec2( x - contentSize.x, 110));
    ImGui::SetNextWindowPos(ImVec2(contentSize.x, 0)); // Set window position to the top-left corner
    ImGui::Begin("plot", nullptr);

    // Update frame time
    static float previousFrameTime = static_cast<float>(glfwGetTime());
    float currentFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;

    // Accumulate frame times
    frameTimeSum += deltaTime;
    frameTimes.push_back(deltaTime);
    if (frameTimeSum > maxHistoryTime) {
        frameTimeSum -= frameTimes.front();
        frameTimes.pop_front();
    }

    // Plot the frame rate graph with scaling for sensitivity
    plotFrameRateGraph();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Ensure the framebuffer size is used correctly
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
}

void ui::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ui::plotFrameRateGraph() {
    glfwGetWindowSize(window, &x, &y);
    ImGui::Text("Frame Rate plot:");

    // Convert frame times to frame rates
    std::vector<float> frameRates(frameTimes.size());
    std::transform(frameTimes.begin(), frameTimes.end(), frameRates.begin(),
        [](float frameTime) { return 1.0f / frameTime; });

    // Aggregate frame rates over small windows
    size_t windowSize = 10; // Size of the window for aggregation
    std::vector<float> aggregatedRates;
    
    for (size_t i = 0; i <= frameRates.size() - windowSize; ++i) {
        float sum = std::accumulate(frameRates.begin() + i, frameRates.begin() + i + windowSize, 0.0f);
        aggregatedRates.push_back(sum / windowSize);
    }

    // Plot aggregated frame rates with adjusted Y-axis range
    float maxRate = *std::max_element(aggregatedRates.begin(), aggregatedRates.end());
    ImGui::PlotLines("", aggregatedRates.data(), aggregatedRates.size(), 0, nullptr, 0.0f, maxRate * 1.2f, ImVec2(x-400,60));
}

