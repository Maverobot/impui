#include <imgui_plot.h>
#include <impui/canvas.h>
#include <impui/simple_plotter.h>

#include <cmath>
#include <iostream>

#include <cstdio>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " buffer_size[int]" << std::endl;
    return 0;
  }
  impui::SimplePlotter plotter(atoi(argv[1]));

  bool show_plot = false;
  float value = 0.001;

  // Main loop
  for (;;) {
    // update data
    plotter.add({{"position", std::sin(value)},
                 {"velocity", std::cos(value)},
                 {"acceleration", -std::sin(value)}});
    Eigen::Matrix<double, 5, 1> mat = Eigen::Matrix<double, 5, 1>::Random();
    plotter.add("eigen_vec", mat);
    plotter.plot();
    value += 0.1f;
  }

  return 0;
}
