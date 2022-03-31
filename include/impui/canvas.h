#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>

namespace impui {
class Canvas {
 public:
  explicit Canvas(int width,
                  int height,
                  const char* title,
                  GLFWmonitor* monitor,
                  GLFWwindow* share) {
    // Setup glfw window
    glfwSetErrorCallback(glfwErrorCallback);
    if (glfwInit() == 0) {
      throw std::runtime_error("glfw initialization failed.");
    }
    window_ptr_ = glfwCreateWindow(width, height, title, monitor, share);
    if (window_ptr_ == nullptr) {
      throw std::runtime_error("glfwCreateWindow failed.");
    }

    glfwMakeContextCurrent(window_ptr_);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
    // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_ptr_, true);
    ImGui_ImplOpenGL2_Init();
  }

  ~Canvas() {
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window_ptr_);
    glfwTerminate();
  }

  auto shouldClose() const -> bool { return glfwWindowShouldClose(window_ptr_) != 0; }

  static void poll() { glfwPollEvents(); }

  static void frameStart() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void frameEnd() const {
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window_ptr_);
    glfwSwapBuffers(window_ptr_);
  }

  template <typename Fn>
  void showWindow(char const* name, Fn&& fn, bool* open = nullptr, ImGuiWindowFlags flags = 0) {
    ImGui::Begin(name, open, flags);  // Create a window called $name
    std::forward<Fn>(fn)();
    ImGui::End();
  }

  static void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
  }

 private:
  static void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  }

  GLFWwindow* window_ptr_{nullptr};
};
}  // namespace impui
