
#include <imgui_plot.h>
#include <impui/canvas.h>

#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <vector>

#include <cstdio>

template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& arr) {
  copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

class PlotData {
 public:
  explicit PlotData(size_t buffer_size = 100) : buffer_size_(buffer_size) {}
  void append(const std::string& name, float value) {
    if (!exist(name)) {
      map_.insert(std::make_pair(name, Container{}));
    }
    auto& data = map_.at(name);
    data.push_back(value);
    if (data.size() > buffer_size_) {
      data.erase(data.begin());
    }
  }

  float const* data(const std::string& name) const {
    if (!exist(name)) {
      return nullptr;
    }

    return map_.at(name).data();
  }

  void clearAll() {
    for (auto& pair : map_) {
      pair.second.clear();
    }
  }

  void clear(const std::string& name) {
    if (!exist(name)) {
      throw std::invalid_argument("no key found!");
    }
    map_.at(name).clear();
  }

  size_t len(const std::string& name) const {
    if (!exist(name)) {
      throw std::invalid_argument("no key found!");
    }
    return map_.at(name).size();
  }

  std::string str(const std::string& name) const {
    if (!exist(name)) {
      return {};
    }

    auto& data = map_.at(name);
    std::ostringstream oss;
    oss << "[ ";
    oss.precision(6);
    oss << data;
    oss << "]";
    return oss.str();
  }

 private:
  using Container = std::vector<float>;
  using DataMap = std::map<std::string, Container>;

  DataMap map_;
  size_t buffer_size_;

  bool exist(const std::string& name) const { return map_.find(name) != map_.end(); }
};

int main(int /*unused*/, char** /*unused*/) {
  impui::Canvas canvas(1280, 720, "Simple Impui plot example", nullptr, nullptr);
  if (canvas.get() == nullptr) {
    return 1;
  }

  PlotData data_map(100);

  bool show_plot = false;
  float value = 0.001;
  // Main loop
  while (!canvas.shouldClose()) {
    // update data
    data_map.append("position", std::sin(value));
    data_map.append("velocity", std::cos(value));
    data_map.append("acceleration", -std::sin(value));
    value += 0.1f;

    canvas.poll();
    canvas.frameStart();
    // 1. Show a simple window
    canvas.showWindow("ImViz", [&data_map, &show_plot] {
      static float f = 0.0f;

      ImGui::Text("This is a simple visualization demo.");

      ImGui::SliderFloat("float", &f, 0.0f,
                         1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f

      if (ImGui::Button("Reset")) {
        data_map.clearAll();
      }

      ImGui::Checkbox("show plot", &show_plot);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    });

    // 2. Show another simple window.
    if (show_plot) {
      canvas.showWindow(
          "Plot",
          [&data_map] {
            ImGui::PlotLines("position", data_map.data("position"), data_map.len("position"), 0,
                             nullptr, -1, 1, ImVec2(0, 80));

            ImGui::PlotLines("velocity", data_map.data("velocity"), data_map.len("velocity"), 0,
                             nullptr, -1, 1, ImVec2(0, 80));

            ImGui::PlotLines("acceleration", data_map.data("acceleration"),
                             data_map.len("acceleration"), 0, nullptr, -1, 1, ImVec2(0, 80));
          },
          &show_plot, ImGuiWindowFlags_AlwaysAutoResize);
    }

    canvas.render();
    canvas.frameEnd();
  }

  return 0;
}
