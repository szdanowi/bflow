#pragma once

#include <functional>
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

template <typename event_t, typename subflow_t>
class subflow_step : public linked_step<event_t>
{
public:
  subflow_step(subflow_t&& subflow, typename linked_step<event_t>::ptr&& next)
      : linked_step<event_t>(std::move(next)), _subflow(std::move(subflow)) {}

  result process(event_t e) override { return _process<subflow_t>(e); }

  explicit operator bool() const override { return true; }

private:
  template <typename object_t>
  inline auto _process(event_t e) -> decltype(std::declval<object_t>().process(std::declval<event_t>()), result()) {
    return _subflow.process(e);
  }

  template <typename object_t>
  inline auto _process(event_t e) -> decltype(std::declval<object_t>()(std::declval<event_t>()), result()) {
    return _subflow(e);
  }

  subflow_t _subflow;
};

template <typename event_t, typename selection_t>
class steps_selection : public linked_step<event_t>
{
public:
  template <typename... steps_t>
  static steps_selection<event_t, selection_t> of(steps_t&&... steps);

  result process(event_t e) override {
    auto outcome = bflow::result::rejected;
    for (auto& step : _steps) {
      auto result = step.process(e);
      if (result == bflow::result::completed) return result;
      if (result == bflow::result::accepted) outcome = result;
    }
    return outcome;
  }

private:
  using steps = detail::steps<event_t>;

  explicit steps_selection(steps&&);

  steps _steps;
};

template <typename event_t, typename subflow_t>
auto create_step(subflow_t&& subflow, typename linked_step<event_t>::ptr&& next) {
  return std::make_unique<subflow_step<event_t, subflow_t>>(std::move(subflow), std::move(next));
}

} // namespace bflow::detail
