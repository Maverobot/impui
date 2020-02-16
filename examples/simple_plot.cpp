#include <imgui_plot.h>
#include <impui/canvas.h>
#include <impui/simple_plotter.h>

#include <chrono>
#include <cmath>
#include <iostream>

#include <cstdio>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " buffer_size[int]" << std::endl;
    return 0;
  }
  impui::SimplePlotter plotter;
  plotter.setBufferSize(atoi(argv[1]));

  bool show_plot = false;
  float value = 0.001;

  // Main loop
  auto last = std::chrono::steady_clock::now();
  for (;;) {
    // update data
    plotter.add({{"position", std::sin(value)},
                 {"velocity", std::cos(value)},
                 {"acceleration", -std::sin(value)}});
    Eigen::Matrix<double, 5, 1> mat = Eigen::Matrix<double, 5, 1>::Random();
    plotter.add("eigen_vec", mat);
    plotter.plot();
    value += 0.1f;
    auto now = std::chrono::steady_clock::now();
    auto interval =
        std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(now - last)
            .count();
    last = now;
    std::cout << "loop frequency: " << 1 / interval << std::endl;
  }

  return 0;
}
