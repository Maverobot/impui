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
  SimplePlotter() = default;

  SimplePlotter(size_t size);

  void add(std::vector<std::pair<std::string, double>> name_value_pairs);

  template <typename Derived>
  void add(const std::string& key, const Eigen::PlainObjectBase<Derived>& matrix) {
    int i = 0;
    for (auto it = Not_Eigen::begin(matrix); it < Not_Eigen::end(matrix); it++) {
      data_.append(key + std::to_string(i), *it);
      i++;
    }
  }

  void plot();

 private:
  void show_main_window();

  impui::PlotOptions options_;
  impui::PlotData data_{100};
  bool show_plot_{true};

  std::unique_ptr<impui::Canvas> canvas_ptr_;
};
}  // namespace impui
