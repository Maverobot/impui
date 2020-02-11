#pragma once

#include <map>
#include <vector>

namespace impui {
template <typename Key,
          typename Tp,
          typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, Tp>>>
class sorted_map : public std::map<Key, Tp, Compare, Alloc> {
 public:
  template <typename Pair>
  auto insert_with_key(Pair&& x) {
    keys_.push_back(x.first);
    return std::map<Key, Tp, Compare, Alloc>::insert(std::forward<Pair>(x));
  }

  std::vector<Key> const& keys() const { return keys_; }

 private:
  std::vector<Key> keys_;
};
}  // namespace impui
