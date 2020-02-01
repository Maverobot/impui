#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#include <memory>

namespace impui {
class Canvas {
 public:
  template <typename... Args>
  explicit Canvas(Args&&... args) {
    // Setup glfw window
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
      throw std::runtime_error("glfw initialization failed.");
    }
    window_ptr_ = glfwCreateWindow(std::forward<Args>(args)...);
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

  ~Canvas();

  bool shouldClose() const;

  void poll();

  void frameStart();

  void frameEnd() const;

  template <typename Fn>
  void showWindow(char const* name, Fn&& fn, bool* open = nullptr, ImGuiWindowFlags flags = 0) {
    ImGui::Begin(name, open, flags);  // Create a window called $name
    std::forward<Fn>(fn)();
    ImGui::End();
  }

  void render();

 private:
  static void glfwErrorCallback(int error, const char* description);

  GLFWwindow* window_ptr_{nullptr};
};
}  // namespace impui
