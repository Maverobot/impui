#pragma once

#include <impui/canvas.h>
#include <impui/plot.hpp>
#include <impui/plot_data.hpp>
#include <memory>

namespace impui {
class SimplePlotter {
 public:
  SimplePlotter(size_t size = 100)
      : data(size),
        canvas_ptr_(std::make_unique<impui::Canvas>(1280,
                                                    720,
                                                    "Simple Impui plot example",
                                                    nullptr,
                                                    nullptr)) {
    options.graph_size(ImVec2(0, 80));
  }

  using NameValuePair = std::pair<std::string, double>;
  void plot(std::vector<NameValuePair> name_value_pairs) {
    if (!canvas_ptr_ || canvas_ptr_->shouldClose()) {
      canvas_ptr_.reset();
      return;
    }

    for (auto pair : name_value_pairs) {
      data.append(pair.first, pair.second);
    }

    canvas_ptr_->poll();
    canvas_ptr_->frameStart();

    // 1. Show a simple window
    show_main_window();

    // 2. Show another simple window.
    if (show_plot) {
      canvas_ptr_->showWindow("Plot",
                              [this, &name_value_pairs] {
                                for (auto pair : name_value_pairs) {
                                  impui::plot(data, pair.first, options);
                                }
                              },
                              &show_plot, ImGuiWindowFlags_AlwaysAutoResize);
    }

    canvas_ptr_->render();
    canvas_ptr_->frameEnd();
  }

 private:
  void show_main_window() {
    canvas_ptr_->showWindow("ImViz", [this] {
      if (ImGui::Button("Reset")) {
        data.clearAll();
      }
      ImGui::SameLine();
      ImGui::Checkbox("show plot", &show_plot);
    });
  }

  void show_plot_window() {}

  impui::PlotOptions options;
  impui::PlotData data;
  bool show_plot{true};

  std::unique_ptr<impui::Canvas> canvas_ptr_;
};
}  // namespace impui
