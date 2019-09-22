#pragma once

#include "flow_result.hpp"
#include <memory>
#include <stdexcept>
#include <utility>

namespace bflow::detail
{
struct invalid_step : public std::runtime_error
{
  invalid_step() : std::runtime_error("Attempted to use a step that does not exist") {}
};

template <typename event_t>
class linked_step
{
public:
  using self_t = linked_step<event_t>;
  using ptr = std::unique_ptr<self_t>;
  static self_t universal_end;

  linked_step() = default;
  explicit linked_step(ptr&& next) : _next(std::move(next)) {}

  virtual result process(event_t) { throw invalid_step(); }
  self_t& next() { return _next ? *_next : universal_end; }

  virtual explicit operator bool() const { return false; }

  bool operator==(const self_t& other) const { return this == &other; }
  bool operator!=(const self_t& other) const { return this != &other; }

private:
  std::unique_ptr<self_t> _next = nullptr;
};

template <typename event_t>
linked_step<event_t> linked_step<event_t>::universal_end = {};
} // namespace bflow::detail
