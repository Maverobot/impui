#pragma once

#include "plot_data.hpp"

#include <imgui.h>
#include <string>

namespace impui {

void plot(const PlotData& data,
          const std::string& name,
          int values_offset = 0,
          const char* overlay_text = NULL,
          float scale_min = FLT_MAX,
          float scale_max = FLT_MAX,
          ImVec2 graph_size = ImVec2(0, 0),
          int stride = sizeof(float)) {
  ImGui::PlotLines(name.c_str(), data.data(name), data.len(name), values_offset, overlay_text,
                   scale_min, scale_max, graph_size, stride);
}
}  // namespace impui
