#include <imgui_plot.h>
#include <impui/canvas.h>
#include <impui/simple_plotter.h>

#include <chrono>
#include <cmath>
#include <iostream>

#include <cstdio>

using namespace std::chrono_literals;

auto main(int argc, char* argv[]) -> int {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " buffer_size[int]" << std::endl;
    return 0;
  }
  impui::SimplePlotter plotter;
  plotter.setBufferSize(atoi(argv[1]));

  // bool show_plot = false;
  float value = 0.001;

  // Main loop
  auto last = std::chrono::steady_clock::now();
  for (;;) {
    // update data
    plotter.add({{"position", std::sin(value / 3)},
                 {"velocity", std::cos(value / 3)},
                 {"acceleration", -std::sin(value / 3)}});
    Eigen::Matrix<double, 5, 1> mat = Eigen::Matrix<double, 5, 1>::Random();
    plotter.add("eigen_vec", mat);
    plotter.plot();
    value += 0.1F;
    auto now = std::chrono::steady_clock::now();
    auto interval =
        std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(now - last)
            .count();
    last = now;
    std::cout << "loop frequency: " << 1 / interval << std::endl;
  }

  return 0;
}
