#include <impui/simple_plotter.h>
#include <Eigen/Dense>

namespace impui {
SimplePlotter::SimplePlotter(size_t size)
    : data_(size),
      canvas_ptr_(std::make_unique<impui::Canvas>(1280,
                                                  720,
                                                  "Simple Impui plot example",
                                                  nullptr,
                                                  nullptr)) {
  options_.graph_size(ImVec2(0, 80));
}

void SimplePlotter::add(std::vector<std::pair<std::string, double>> name_value_pairs) {
  for (const auto& pair : name_value_pairs) {
    data_.append(pair.first, pair.second);
  }
}

void SimplePlotter::plot() {
  if (!canvas_ptr_ || canvas_ptr_->shouldClose()) {
    canvas_ptr_.reset();
    return;
  }

  canvas_ptr_->poll();
  canvas_ptr_->frameStart();

  // 1. Show a simple window
  show_main_window();

  // 2. Show another simple window.
  if (show_plot_) {
    canvas_ptr_->showWindow("Plot",
                            [this] {
                              for (auto& key : data_.keys()) {
                                if (key.empty()) {
                                  return;
                                }
                                impui::plot(data_, key, options_);
                              }
                            },
                            &show_plot_, ImGuiWindowFlags_AlwaysAutoResize);
  }

  canvas_ptr_->render();
  canvas_ptr_->frameEnd();
}

void SimplePlotter::show_main_window() {
  canvas_ptr_->showWindow("ImViz", [this] {
    if (ImGui::Button("Reset")) {
      data_.clearAll();
    }
    ImGui::SameLine();
    ImGui::Checkbox("show plot", &show_plot_);
  });
}
}  // namespace impui
