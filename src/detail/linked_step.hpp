#pragma once

#include "result.hpp"
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
  using self_t = linked_step<event_t>;

public:
  static self_t universal_end;

  linked_step() = default;
  linked_step(self_t&&) = default;
  linked_step(const self_t&) = delete;

  self_t& operator=(self_t&&) = default;
  self_t& operator=(const self_t&) = delete;

  explicit linked_step(std::unique_ptr<self_t>&& next) : _next(std::move(next)) {}
  virtual ~linked_step() = default;

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
