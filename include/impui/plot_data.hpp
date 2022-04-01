#pragma once

#include <algorithm>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <impui/sorted_map.h>

namespace {
template <class T>
auto operator<<(std::ostream& o, const std::vector<T>& arr) -> std::ostream& {
  copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}
}  // namespace

namespace impui {
class PlotData {
 public:
  explicit PlotData(size_t buffer_size = 100) : buffer_size_(buffer_size) {}
  void append(const std::string& name, float value) {
    if (!exist(name)) {
      map_.insertWithKey(std::make_pair(name, Container{}));
    }
    auto& serie = map_.at(name);
    serie.push_back(value);
    if (serie.size() > buffer_size_) {
      std::vector<std::decay_t<decltype(serie)>::value_type>(
          serie.begin() + (serie.size() - buffer_size_), serie.end())
          .swap(serie);
    }
  }

  auto data(const std::string& name) const -> float const* {
    if (!exist(name)) {
      return nullptr;
    }

    return map_.at(name).data();
  }

  void setBufferSize(size_t buffer_size) { buffer_size_ = buffer_size; }

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

  auto len(const std::string& name) const -> size_t {
    if (!exist(name)) {
      throw std::invalid_argument("no key found!");
    }
    return map_.at(name).size();
  }

  auto str(const std::string& name) const -> std::string {
    if (!exist(name)) {
      return {};
    }

    const auto& serie = map_.at(name);
    std::ostringstream oss;
    oss << "[ ";
    oss.precision(6);
    oss << serie;
    oss << "]";
    return oss.str();
  }

  auto keys() const -> auto const& { return map_.keys(); }

 private:
  using Container = std::vector<float>;
  using DataMap = impui::SortedMap<std::string, Container>;

  DataMap map_;
  size_t buffer_size_;

  auto exist(const std::string& name) const -> bool { return map_.find(name) != map_.end(); }
};
}  // namespace impui
