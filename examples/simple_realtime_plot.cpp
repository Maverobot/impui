#include <imgui_plot.h>
#include <impui/canvas.h>
#include <impui/simple_realtime_plotter.h>

#include <chrono>
#include <cmath>
#include <iostream>

#include <cstdio>

const double kPeriodScaler = 1000;
const size_t kRandomMatrixRowsNumber = 5;
const size_t kRandomMatrixColumnsNumber = 1;
using ExampleMatrix = Eigen::Matrix<double, kRandomMatrixRowsNumber, kRandomMatrixColumnsNumber>;

using namespace std::chrono_literals;

auto main(int argc, char* argv[]) -> int {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " buffer_size[int]" << std::endl;
    return 0;
  }
  impui::SimpleRealtimePlotter plotter;
  // plotter.setBufferSize(atoi(argv[1]));

  // bool show_plot = false;
  float steps = 0;

  // Main loop
  auto last = std::chrono::steady_clock::now();
  for (;;) {
    // Update data
    plotter.add({{"position", std::sin(steps / kPeriodScaler)},
                 {"velocity", std::cos(steps / kPeriodScaler)},
                 {"acceleration", -std::sin(steps / kPeriodScaler)}});
    ExampleMatrix mat = ExampleMatrix::Random();
    plotter.add("eigen_vec", mat);

    // Plot
    plotter.plot();

    // Show time cost for adding and plotting data
    auto now = std::chrono::steady_clock::now();
    auto interval =
        std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(now - last).count();
    last = now;
    std::cout << "Time cost for adding/plotting data: " << interval << " milliseconds" << std::endl;

    steps++;
  }

  return 0;
}
