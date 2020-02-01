#include <imgui_plot.h>
#include <impui/canvas.h>
#include <impui/plot.hpp>
#include <impui/plot_data.hpp>

#include <cmath>
#include <iostream>

#include <cstdio>

int main(int /*unused*/, char** /*unused*/) {
  impui::Canvas canvas(1280, 720, "Simple Impui plot example", nullptr, nullptr);
  if (canvas.get() == nullptr) {
    return 1;
  }

  impui::PlotData plot_data(100);

  bool show_plot = false;
  float value = 0.001;
  // Main loop
  while (!canvas.shouldClose()) {
    // update data
    plot_data.append("position", std::sin(value));
    plot_data.append("velocity", std::cos(value));
    plot_data.append("acceleration", -std::sin(value));
    value += 0.1f;

    canvas.poll();
    canvas.frameStart();

    // 1. Show a simple window
    canvas.showWindow("ImViz", [&plot_data, &show_plot] {
      if (ImGui::Button("Reset")) {
        plot_data.clearAll();
      }
      ImGui::SameLine();
      ImGui::Checkbox("show plot", &show_plot);
    });

    // 2. Show another simple window.
    if (show_plot) {
      canvas.showWindow("Plot",
                        [plot_data = std::as_const(plot_data)] {
                          impui::plot(plot_data, "position");
                          impui::plot(plot_data, "velocity");
                          impui::plot(plot_data, "acceleration");
                        },
                        &show_plot, ImGuiWindowFlags_AlwaysAutoResize);
    }

    canvas.render();
    canvas.frameEnd();
  }

  return 0;
}
