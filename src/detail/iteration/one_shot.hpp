#pragma once

#include "result.hpp"

namespace bflow::detail::iteration
{
struct one_shot
{
  template <typename iterator_t, typename step_collection_t>
  static inline result advance(iterator_t& current, result result, const step_collection_t&) {
    if (result != result::completed) return result;
    return ++current ? result::accepted : result::completed;
  }
};
} // namespace bflow::detail::iteration
