#pragma once

#include "result.hpp"

namespace bflow::detail::iteration
{
struct looped
{
  template <typename iterator_t, typename step_collection_t>
  static inline result advance(iterator_t& current, result result, const step_collection_t& steps) {
    if (result != result::completed) return result;
    if (++current) return result::accepted;

    current = steps.begin();
    return result::completed;
  }
};
} // namespace bflow::detail::iteration
