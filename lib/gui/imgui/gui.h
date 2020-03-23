#pragma once

#include <stdio.h>
#include <functional>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>    // Initialize with glewInit()
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

void main_loop(GLFWwindow* window, std::vector<std::function<void()>> const& drawing_procs);


static void glfw_error_callback(int error, const char* description)
{
   fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int start_gui(std::vector<std::function<void()>> const& drawing_procs)
{
   // Setup window
   glfwSetErrorCallback(glfw_error_callback);
   if (!glfwInit())
      return 1;

   const char* glsl_version = "#version 330";
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

   // Create window with graphics context
   GLFWwindow* window = glfwCreateWindow(500, 500, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
   if (window == NULL)
      return 1;
   glfwMakeContextCurrent(window);
   glfwSwapInterval(1); // Enable vsync

   // Initialize OpenGL loader
   bool err = glewInit() != GLEW_OK;

   if (err)
   {
      fprintf(stderr, "Failed to initialize OpenGL loader!\n");
      return 1;
   }

   // Setup Dear ImGui context
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

   // Setup Dear ImGui style
   ImGui::StyleColorsDark();

   // Setup Platform/Renderer bindings
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init(glsl_version);

   
   // Main loop
   main_loop(window, drawing_procs);


   // Cleanup
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}

void main_loop(GLFWwindow* window, std::vector<std::function<void()>> const& drawing_procs){
   // Our state
   bool rewards = true;
   ImVec4 bg_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

   while(!glfwWindowShouldClose(window)){
      // using namespace std::chrono_literals;
      // std::this_thread::sleep_for(10ms); // dont do this

      glfwPollEvents();

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // Call drawing functions
      for(auto const& d : drawing_procs)
         d();

      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(bg_color.x, bg_color.y, bg_color.z, bg_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
   }
}

