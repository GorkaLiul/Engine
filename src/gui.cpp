#include "gui.h"
#include<iostream>
#include <algorithm> // For std::max_element
#include<numeric>
#include <vector>

void topBarmenu();

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
    style.FrameBorderSize = 2; 
    style.FrameRounding = 6;
}


void ui::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ui::mainGui(bool &render, bool &wireframe, float &colors) {
  glfwGetWindowSize(window, &x, &y);
  overlay();
  plotOverlay();
 

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


void ui::overlay(){

    static bool p_open = true;
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    // Determine window position based on location
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

    // Begin the ImGui window
    if (ImGui::Begin("Cursor and Framerate Overlay", &p_open, window_flags))
    {
        ImGui::Text("Cursor and Framerate Overlay\n(right-click to change position)");
        ImGui::Separator();

        // Display mouse position
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");

        // Display framerate
        ImGui::Text("Framerate: %.1f FPS", io.Framerate);

        // Context menu to change overlay position
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
            if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
            if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            if (ImGui::MenuItem("Close")) p_open = false;
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void ui::plotOverlay(){

    static bool p_open = true;
    static std::vector<float> framerates;
    static float time_accumulator = 0.0f;
    static float max_time = 5.0f; // 5 seconds of history

    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    // Accumulate time and store framerates
    time_accumulator += io.DeltaTime;
    if (time_accumulator >= 1.0f / 60.0f) // Update every frame (or every 1/60 second)
    {
        framerates.push_back(io.Framerate);
        time_accumulator = 0.0f;
    }

    // Remove old framerates
    while (framerates.size() > max_time * 60) // assuming 60 FPS
    {
        framerates.erase(framerates.begin());
    }

    // Position the window next to the current overlay
    ImVec2 overlay_pos = ImGui::GetMainViewport()->Pos;
    overlay_pos.x += 260.0f; // Adjust this value as needed to position the window correctly
    ImGui::SetNextWindowPos(overlay_pos, ImGuiCond_Always, ImVec2(0.0f, -0.1f));

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

    if (ImGui::Begin("Framerate Plot Overlay", &p_open, window_flags))
    {
        ImGui::Text("Framerate Plot (last 5 seconds)");

        if (!framerates.empty())
        {
            ImGui::PlotLines("", framerates.data(), framerates.size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 50.0f));
        }
        else
        {
            ImGui::Text("No data yet...");
        }

        // Context menu to close the overlay
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Close")) p_open = false;
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}



