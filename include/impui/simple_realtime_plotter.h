#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <impui/simple_plotter.h>

#include <concurrency_utils/executor.h>

namespace impui {

using FunctorType = std::function<void(void)>;
void run(FunctorType f) {
  f();
}

class SimpleRealtimePlotter {
 public:
  SimpleRealtimePlotter() : executor_(run){};

  void add(std::vector<std::pair<std::string, double>> name_value_pairs) {
    fs_.emplace_back([this, name_value_pairs = std::move(name_value_pairs)] {
      plotter_ptr_->add(name_value_pairs);
    });
  }

  template <typename Matrix_T>
  void add(std::string key, Matrix_T matrix) {
    fs_.emplace_back([this, key, matrix] { plotter_ptr_->add(key, matrix); });
  }

  void plot() {
    executor_.execute([this, fs = std::move(this->fs_)] {
      // static impui::SimplePlotter plotter;
      if (!plotter_ptr_) {
        // ImGui/Canvas has to be created in the same thread as the following calls
        plotter_ptr_ = std::make_unique<impui::SimplePlotter>();
      }

      for (auto f : fs) {
        f();
      }

      plotter_ptr_->plot();
    });
    fs_.clear();
  }

 private:
  concurrency_utils::Executor<FunctorType> executor_;
  std::unique_ptr<impui::SimplePlotter> plotter_ptr_;
  std::vector<FunctorType> fs_;
};
}  // namespace impui
