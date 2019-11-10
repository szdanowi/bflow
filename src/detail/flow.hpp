#pragma once

#include "steps.hpp"
#include "result.hpp"
#include <functional>
#include <utility>

namespace bflow::detail
{
template <typename event_t, typename iteration_t>
class flow
{
  using self_t = flow<event_t, iteration_t>;

public:
  using listener = std::function<void()>;

  flow() = default;
  flow(self_t&&) = default;
  flow(const self_t&) = delete;

  self_t& operator=(self_t&&) = default;
  self_t& operator=(const self_t&) = delete;

  template <typename... steps_t>
  static self_t of(steps_t&&... steps) {
    return self_t(make_steps<event_t>(std::forward<steps_t>(steps)...));
  }

  result process(event_t event) {
    if (not _current) return result::rejected;
    auto result = iteration_t::advance(_current, _current.process(event), _steps);
    if (result == result::completed) _completion_listener();
    return result;
  }

  inline void on_completion(listener completion_listener) { _completion_listener = completion_listener; }

private:
  using current_step = typename steps<event_t>::iterator;

  explicit flow(steps<event_t>&& steps) : _steps(std::move(steps)) {}

  steps<event_t> _steps;
  current_step _current = _steps.begin();
  listener _completion_listener = [] {};
};

} // namespace bflow::detail
