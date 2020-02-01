#pragma once

#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace {
template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& arr) {
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
}  // namespace impui
