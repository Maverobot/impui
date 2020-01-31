#include <impui/canvas.h>

namespace impui {
Canvas::~Canvas() {
  // Cleanup
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window_ptr_);
  glfwTerminate();
}

GLFWwindow* Canvas::get() const noexcept {
  return window_ptr_;
}

bool Canvas::shouldClose() const {
  return glfwWindowShouldClose(window_ptr_) != 0;
}

void Canvas::poll() {
  glfwPollEvents();
}

void Canvas::frameStart() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Canvas::frameEnd() const {
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
  glfwMakeContextCurrent(window_ptr_);
  glfwSwapBuffers(window_ptr_);
}

void Canvas::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui::Render();
}

void Canvas::glfwErrorCallback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
}  // namespace impui
