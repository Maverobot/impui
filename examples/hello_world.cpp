#include <impui/canvas.h>

#include <stdio.h>

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

    // 1. Show a simple window
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

    // 2. Show another simple window.
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

    // Set background color
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

    canvas.render();
    canvas.frame_end();
  }

  return 0;
}
