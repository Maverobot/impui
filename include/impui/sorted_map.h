#pragma once

#include <map>
#include <vector>

namespace impui {
template <typename Key,
          typename Tp,
          typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, Tp>>>
class SortedMap : public std::map<Key, Tp, Compare, Alloc> {
 public:
  template <typename Pair>
  auto insertWithKey(Pair&& x) {
    keys_.push_back(x.first);
    return std::map<Key, Tp, Compare, Alloc>::insert(std::forward<Pair>(x));
  }

  auto keys() const -> std::vector<Key> const& { return keys_; }

 private:
  std::vector<Key> keys_;
};
}  // namespace impui
