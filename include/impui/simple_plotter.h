#pragma once

#include <impui/canvas.h>
#include <impui/plot.hpp>
#include <impui/plot_data.hpp>

#include <Eigen/Dense>
#include <iostream>
#include <memory>

namespace Not_Eigen {
template <typename Derived>
typename Eigen::PlainObjectBase<Derived>::Scalar const* begin(
    const Eigen::PlainObjectBase<Derived>& m) {
  return m.data();
}
template <typename Derived>
typename Eigen::PlainObjectBase<Derived>::Scalar const* end(
    const Eigen::PlainObjectBase<Derived>& m) {
  return m.data() + m.size();
}
}  // namespace Not_Eigen

namespace impui {
class SimplePlotter {
 public:
  SimplePlotter()
      : canvas_ptr_(std::make_unique<impui::Canvas>(1280,
                                                    720,
                                                    "Simple Impui plot example",
                                                    nullptr,
                                                    nullptr)) {
    options_.graph_size(ImVec2(0, 80));
  }

  void add(const std::vector<std::pair<std::string, double>>& name_value_pairs) {
    for (const auto& pair : name_value_pairs) {
      data_.append(pair.first, pair.second);
    }
  }

  template <typename Derived>
  void add(const std::string& key, const Eigen::PlainObjectBase<Derived>& matrix) {
    int i = 0;
    for (auto it = Not_Eigen::begin(matrix); it < Not_Eigen::end(matrix); it++) {
      data_.append(key + std::to_string(i), *it);
      i++;
    }
  }

  void setBufferSize(size_t buffer_size) { data_.setBufferSize(buffer_size); }

  void plot() {
    if (!canvas_ptr_ || canvas_ptr_->shouldClose()) {
      canvas_ptr_.reset();
      return;
    }

    canvas_ptr_->poll();
    canvas_ptr_->frameStart();

    // 1. Show a simple window
    showMainWindow();

    // 2. Show another simple window.
    if (show_plot_) {
      canvas_ptr_->showWindow(
          "Plot",
          [this] {
            auto& data = paused_ ? data_snap_ : data_;
            for (auto& key : data.keys()) {
              if (key.empty()) {
                throw std::runtime_error("key must not be empty!");
              }
              ImVec2 window_size = ImGui::GetWindowSize();
              // TODO: fix this hacky line
              options_.graph_size({window_size.x - 100, window_size.y / data.keys().size() - 10});
              data_.setBufferSize(buffer_size_);
              impui::plot(data, key, options_, impui::UseNativePlot{});
            }
          },
          &show_plot_);
    }

    canvas_ptr_->render();
    canvas_ptr_->frameEnd();
  }

 private:
  void showMainWindow() {
    canvas_ptr_->showWindow("ImViz",
                            [this] {
                              if (ImGui::Button("Reset")) {
                                data_.clearAll();
                              }
                              ImGui::SameLine();
                              if (!paused_ && ImGui::Button("Snapshot")) {
                                paused_ = true;
                                data_snap_ = data_;
                              } else if (paused_ && ImGui::Button("Continue")) {
                                paused_ = false;
                              }
                              ImGui::Checkbox("show plot", &show_plot_);
                              ImGui::Text("Buffer size: ");
                              ImGui::SameLine();
                              ImGui::SliderInt("", &buffer_size_, 1, 1000);
                            },
                            nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  }

  impui::PlotOptions options_;
  impui::PlotData data_{100};
  bool show_plot_{true};

  // For pausing the plot
  bool paused_{false};
  impui::PlotData data_snap_;

  int buffer_size_{100};

  std::unique_ptr<impui::Canvas> canvas_ptr_;
};
}  // namespace impui
