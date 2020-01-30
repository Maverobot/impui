#include <impui/canvas.h>

#include <stdio.h>

int main(int, char **) {
  impui::Canvas canvas(1280, 720, "Simple Impui plot example", nullptr,
                       nullptr);
  if (canvas.get() == nullptr) {
    return 1;
  }

  bool show_plot = false;
  // Main loop
  while (!canvas.shouldClose()) {
    canvas.poll();
    canvas.frame_start();

    // 1. Show a simple window
    canvas.show_window("ImViz", [&show_plot] {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Text("This is some useful text."); // Display some text (you can
                                                // use a format strings too)
      ImGui::Checkbox("show plot", &show_plot);

      ImGui::SliderFloat("float", &f, 0.0f,
                         1.0f); // Edit 1 float using a slider from 0.0f to 1.0f

      if (ImGui::Button("Button")) {
        // Buttons return true when clicked (most
        // widgets return true when edited/activated)
        counter++;
      }
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);
    });

    // 2. Show another simple window.
    if (show_plot) {
      canvas.show_window("plot",
                         [&show_plot] {
                           // window will have a closing button that will
                           // clear the bool when clicked)
                           ImGui::Text("Placeholder for plot.");
                           if (ImGui::Button("Close Me")) {
                             show_plot = false;
                           }
                         },
                         &show_plot);
    }

    glClearColor(50.0f / 255.0f, 200.0f / 255.0f, 100.0f / 255.0f, 1.0f);

    canvas.render();
    canvas.frame_end();
  }

  return 0;
}
