
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#include <memory>
#include <stdio.h>

namespace impui {
class Canvas {
public:
  template <typename... Args> Canvas(Args &&... args) {
    // Setup glfw window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
      throw std::runtime_error("glfw initialization failed.");
    }
    window_ptr_ = glfwCreateWindow(std::forward<Args>(args)...);

    glfwMakeContextCurrent(window_ptr_);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
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

  GLFWwindow *get() const noexcept { return window_ptr_; }

  bool shouldClose() const { return glfwWindowShouldClose(window_ptr_); }

  static void poll() { glfwPollEvents(); }

  static void frame_start() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void frame_end() const {
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window_ptr_);
    glfwSwapBuffers(window_ptr_);
  }

  template <typename Fn>
  static void show_window(char const *name, Fn &&fn, bool *open = nullptr) {
    ImGui::Begin(name, open); // Create a window called $name
    std::forward<Fn>(fn)();
    ImGui::End();
  }

  static void render() { ImGui::Render(); }

private:
  static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  }

  GLFWwindow *window_ptr_{nullptr};
};
} // namespace impui

int main(int, char **) {
  impui::Canvas canvas(1280, 720, "Dear ImGui GLFW+OpenGL2 example", nullptr,
                       nullptr);
  if (canvas.get() == nullptr) {
    return 1;
  }

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  while (!canvas.shouldClose()) {
    canvas.poll();
    canvas.frame_start();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to created a named window.

    canvas.show_window("Hello, world!", [&show_another_window, &clear_color] {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Text("This is some useful text."); // Display some text (you can
                                                // use a format strings too)
      ImGui::Checkbox("Another Canvas", &show_another_window);

      ImGui::SliderFloat("float", &f, 0.0f,
                         1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3(
          "clear color",
          (float *)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button")) {
        // Buttons return true when clicked (most
        // widgets return true when edited/activated)
        counter++;
      }
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    });

    // 3. Show another simple window.
    if (show_another_window) {
      canvas.show_window("Another window",
                         [&show_another_window] {
                           // window will have a closing button that will
                           // clear the bool when clicked)
                           ImGui::Text("Hello from another window!");
                           if (ImGui::Button("Close Me")) {
                             show_another_window = false;
                           }
                         },
                         &show_another_window);
    }

    int display_w, display_h;
    glfwGetFramebufferSize(canvas.get(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    // Set background color
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    canvas.render();
    canvas.frame_end();
  }

  return 0;
}
