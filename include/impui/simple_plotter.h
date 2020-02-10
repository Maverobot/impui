#pragma once

#include <impui/canvas.h>
#include <impui/plot.hpp>
#include <impui/plot_data.hpp>
#include <memory>

namespace impui {
class SimplePlotter {
 public:
  SimplePlotter() = default;

  SimplePlotter(size_t size);

  void plot(std::vector<std::pair<std::string, double>> name_value_pairs);

 private:
  void show_main_window();

  impui::PlotOptions options;
  impui::PlotData data{100};
  bool show_plot{true};

  std::unique_ptr<impui::Canvas> canvas_ptr_;
};
}  // namespace impui
