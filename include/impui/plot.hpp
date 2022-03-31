#pragma once

#include <impui/arg.h>
#include <impui/plot_data.hpp>

#include <imgui.h>
#include <imgui_plot.h>

#include <string>

namespace impui {

struct PlotOptions {
  PlotOptions() = default;
  virtual ~PlotOptions() = default;
  /// The number of features of a single sample in the input sequence `x`.
  IMPUI_ARG(int, values_offset) = 0;
  IMPUI_ARG(std::string, overlay_text) = {};
  IMPUI_ARG(float, scale_min) = FLT_MAX;
  IMPUI_ARG(float, scale_max) = FLT_MAX;
  IMPUI_ARG(ImVec2, graph_size) = ImVec2(0, 0);
  IMPUI_ARG(int, stride) = sizeof(float);
};

struct UseImGuiPlot {};
struct UseNativePlot {};

inline void plot(const PlotData& data,
                 const std::string& name,
                 const PlotOptions& options,
                 UseNativePlot /*unused*/) {
  ImGui::PlotLines(name.c_str(), data.data(name), data.len(name), options.values_offset(),
                   options.overlay_text().c_str(), options.scale_min(), options.scale_max(),
                   options.graph_size(), options.stride());
}

inline void plot(const PlotData& data,
                 const std::string& name,
                 const PlotOptions& options,
                 UseImGuiPlot /*unused*/) {
  ImGui::PlotConfig conf;
  conf.values.ys = data.data(name);
  conf.values.count = data.len(name);
  conf.scale.min = -1;
  conf.scale.max = 1;
  conf.tooltip.show = true;
  conf.grid_x.show = false;
  conf.grid_y.show = false;
  conf.frame_size = options.graph_size();
  conf.line_thickness = 2.F;
  conf.overlay_text = options.overlay_text().c_str();

  ImGui::Plot(name.c_str(), conf);
}
}  // namespace impui
