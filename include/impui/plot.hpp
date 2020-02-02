#pragma once

#include <impui/arg.h>
#include <impui/plot_data.hpp>

#include <imgui.h>

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

inline void plot(const PlotData& data, const std::string& name, const PlotOptions& options) {
  ImGui::PlotLines(name.c_str(), data.data(name), data.len(name), options.values_offset(),
                   options.overlay_text().c_str(), options.scale_min(), options.scale_max(),
                   options.graph_size(), options.stride());
}
}  // namespace impui
